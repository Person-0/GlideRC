#include <stdio.h>
#ifdef ON_FC
    #include "pico/stdlib.h"
    #include "hardware/uart.h"
    #include "hardware/irq.h"
#else
    #include <stdint.h>
#endif

#include "hardware.h"

/*
 PIN DEFINITIONS
*/

// Receiver (SBUS protocol)
#define SBUS_PIN 1 // uart rx
#define SBUS_UART_ID uart0

// IMU
#define IMU_SDO 4
#define IMU_NCS 5
#define IMU_SCL 6
#define IMU_SDA 7

// Motor
#define ESC_SIGNAL_PIN 8

// Control surfaces
#define AILERON_SERVO_PIN 9
#define ELEVATOR_SERVO_PIN 10

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

static void parse_channel(volatile uint8_t channel_data[11]) {

    uint16_t value = 0;

    for (int i = 0; i < 11; i++) {
        value |= (channel_data[i] << i);
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

void inject_sbus_byte(uint8_t data) {
    _parse_sbus_byte(data);
}

/*  
=============
=============
*/