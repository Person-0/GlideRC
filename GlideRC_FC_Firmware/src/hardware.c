#include <stdio.h>
#include <stdint.h>
#ifdef ON_FC
    #include "pico/stdlib.h"
    #include "hardware/uart.h"
    #include "hardware/irq.h"
    #include "hardware/pwm.h"
    #include "hardware/clocks.h"
    #include "../libs/mp92plus/inc/mp92plus.h"
#endif
#include "hardware.h"
#include "config.h"
#include "servo.h"

/*  
=============  
=============
 RECEIVER
 SBUS (already externally inverted)
=============
*/

// SBUS settings
#define SBUS_UART_IRQ SBUS_UART_ID == uart0 ? UART0_IRQ : UART1_IRQ
#define SBUS_BAUD_RATE 100000
#define SBUS_DATA_BITS 8
#define SBUS_STOP_BITS 2
#define SBUS_PARITY UART_PARITY_EVEN

// SBUS parsing
volatile uint8_t sbus_index = 0;
volatile uint8_t sbus_bitStore[11];
volatile uint8_t sbus_bitsStored = 0;
volatile uint8_t sbus_workingBit;
volatile uint8_t sbus_channelIndex = 0;
static void (*channel_callback)(uint8_t, uint16_t) = NULL;

static void parse_channel(volatile uint8_t channel_data[11]) {

    uint16_t value = 0;

    for (int i = 0; i < 11; i++) {
        value |= (channel_data[i] << i);
    }

    if (channel_callback) {
        channel_callback(sbus_channelIndex, value);
    } else {
        printf("receiver: channel callback not registered!\n");
    }

    sbus_channelIndex += 1;

    #ifndef ON_FC
        printf("== CHANNEL %d VALUE: %d ==\n", sbus_channelIndex-1, value);
    #endif
}

static void _parse_sbus_byte(volatile uint8_t sbus_byte) {

    #ifndef ON_FC
       printf("sbus byte recieved: %u\n", sbus_byte);
    #endif

    if (sbus_index > 0) {

        if (sbus_index > 22) {

            // 23 = flag
            // 24 = end  0x00

        } else {

            // 16 rc channels on 22 bytes 
            // each channel made of  11 bits
            
            for (int i = 7; i >= 0; i -= 1) {
                sbus_workingBit = (sbus_byte >> i) & 1;
                sbus_bitStore[10 - sbus_bitsStored] = sbus_workingBit;
                sbus_bitsStored += 1;
                if (sbus_bitsStored >= 11) {
                    parse_channel(sbus_bitStore);
                    sbus_bitsStored = 0;
                }
            }

        }

        sbus_index += 1;
        if (sbus_index >= 25) {
            sbus_index = 0;
            sbus_channelIndex = 0;
            sbus_bitsStored = 0;

            #ifndef ON_FC
                printf("finished 25 bytes for an sbus packet\n");
            #endif
        }

    } else if (sbus_byte == 0x0F) {
        sbus_index = 1;
        sbus_channelIndex = 0;
        sbus_bitsStored = 0;

        #ifndef ON_FC
            printf("starting sbus packet parsing (received start byte)\n");
        #endif
    }

}

static void on_uart_rx() {
    #ifdef ON_FC
        while (uart_is_readable(SBUS_UART_ID)) {
            _parse_sbus_byte(uart_getc(SBUS_UART_ID));
        }
    #endif
}

void setup_sbus_uart() {
    #ifdef ON_FC
        uart_init(SBUS_UART_ID, SBUS_BAUD_RATE);
        gpio_set_function(SBUS_PIN, GPIO_FUNC_UART);
        uart_set_hw_flow(SBUS_UART_ID, false, false);
        uart_set_format(
            SBUS_UART_ID,
            SBUS_DATA_BITS,
            SBUS_STOP_BITS,
            SBUS_PARITY
        );
        uart_set_fifo_enabled(SBUS_UART_ID, true);
        irq_set_exclusive_handler(SBUS_UART_IRQ, on_uart_rx);
        irq_set_enabled(SBUS_UART_IRQ, true);
        uart_set_irq_enables(SBUS_UART_ID,true,false); // RX only
        printf("SBUS UART port setup correctly\n");
    #endif
}

void register_channel_callback(void (*callback)(uint8_t, uint16_t)) {
    channel_callback = callback;
    printf("receiver: channel callback registered\n");
}

void inject_sbus_byte(uint8_t data) {
    _parse_sbus_byte(data);
}

/*  
=============
=============
 Control Surfaces
 Servos
=============
*/

void setup_control_servos() {
    setup_servo(AILERON_SERVO_PIN, SERVO_FREQ, SERVO_MID);
    setup_servo(ELEVATOR_SERVO_PIN, SERVO_FREQ, SERVO_MID);
}

void disable_control_servos() {
    disable_servo(AILERON_SERVO_PIN);
    disable_servo(ELEVATOR_SERVO_PIN);
}

void set_aileron_angle(int angle) {
    servo_move(AILERON_SERVO_PIN, angle, SERVO_MIN, SERVO_MAX);
}

void set_elevator_angle(int angle) {
    servo_move(ELEVATOR_SERVO_PIN, angle, SERVO_MIN, SERVO_MAX);
}

/*
=============
=============
 Brushless Motor
=============
*/

void setup_motor() {
    printf("Setting up motor at Pin %d\n", ESC_SIGNAL_PIN);
    setup_servo(ESC_SIGNAL_PIN, ESC_FREQ, ESC_MIN);
}

void disable_motor() {
    printf("Disabling motor at Pin %d\n", ESC_SIGNAL_PIN);
    disable_servo(ESC_SIGNAL_PIN);
}

/*
 @param throttle -> [0, 100]
*/
void set_throttle(uint8_t throttle) {
    throttle = throttle > 100 ? 100 : throttle;
    throttle = (uint8_t)(throttle * ESC_LIMIT);
    servo_move(ESC_SIGNAL_PIN, (throttle * 180) / 100, ESC_MIN, ESC_MAX);
}

/*
=============
=============
 IMU
=============
*/
#ifdef ON_FC
    mpu9250_t IMU;
#endif

/*
 @return 0 -> success,
         1 -> not on fc,
         2 -> mpu9250_reset failed,
         3 -> mpu9250_setup failed.
*/
int setup_imu() {
    #ifdef ON_FC

        spi_pin_t spi_pindefs = {
            .miso = IMU_SDO,
            .mosi = IMU_SDA,
            .sck = IMU_SCL,
            .cs = IMU_NCS,
        };

        uint8_t activate = mpu9250_reset(&IMU, &spi_pindefs, IMU_SPI, 1000 * 1000);

        if (activate > 0) {
            printf("Failed to initialize IMU!\n");
            return 2;
        }

        mpu_settings_t set = {
            .accel_range = ACCEL_RANGE_16G,
            .gyro_range = GYRO_RANGE_2000DPS,
            .dlpf_filter = DLPF_41HZ,
            .sample_rate_divider = 249
        };

        int16_t correct_gyro[] = {0, 0, 0};
        uint8_t settings = mpu9250_setup(&IMU, &set, correct_gyro);

        if (settings) {
            printf("Failed to initialize IMU!\n");
            return 3;
        }

        return 0;

    #endif
    return 1;
}

/*
 @param imu_data output array in which the float values are stored:
                 `{accel[0], accel[1], accel[2], gyro[0] gyro[1] gyro[2]}`
*/
void read_imu(float imu_data[3]) {

    float accel[3] = {0.0f, 0.0f, 0.0f};
    float gyro[3] = {0.0f, 0.0f, 0.0f};

    #ifdef ON_FC

        mpu9250_read_motion(&IMU, accel, gyro);

        printf(
            "Acceleration in G     X = %10.4f,  Y = %10.4f,  Z = %10.4f\n",
            accel[0],
            accel[1],
            accel[2]
        );
        printf("Gyroscope in Deg/s    X = %10.4f,  Y = %10.4f,  Z = %10.4f\n",
            gyro[0],
            gyro[1],
            gyro[2]
        );

    #else
        printf("read_imu() called but not ON_FC\n");
    #endif

    imu_data[0] = accel[0];
    imu_data[1] = accel[1];
    imu_data[2] = accel[2];
    imu_data[3] = gyro[0];
    imu_data[4] = gyro[1];
    imu_data[5] = gyro[2];
}

float read_imu_temp() {

    float temp = 0.0f;

    #ifdef ON_FC

        mpu9250_read_temperature(&IMU, &temp);

        printf("Temperature in C      %10.4f\n\n", temp);

    #else
        printf("read_imu_temp() called but not ON_FC\n");
    #endif

    return temp;
}