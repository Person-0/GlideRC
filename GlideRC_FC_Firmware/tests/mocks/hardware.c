#include <stdio.h>

#ifdef ON_FC
    #include "pico/stdlib.h"
    printf("This isnt supposed to be running ON_FC!!!!!\n");
    while (1) sleep_ms(1000);
#else:
    #include <stdint.h>
    #include "hardware.h"
#endif

// Receiver

static void (*channel_callback)(uint8_t, uint16_t) = NULL;

int setup_sbus_uart() {
    printf("SBUS UART port setup correctly\n");
    return 0;
}

void inject_sbus_byte(uint8_t data) {
    // pass;
}

void register_channel_callback(void (*callback)(uint8_t, uint16_t)) {
    channel_callback = callback;
}

// Control servos

int control_servos_disabled = 0;

int setup_control_servos() {
    control_servos_disabled = 0;
    printf("Control servos setup correctly\n");
    return 0;
}

void disable_control_servos() {
    printf("Control servos disabled\n");
    control_servos_disabled = 1;
}

void set_aileron_angle(int angle) {
    if (control_servos_disabled) {
        printf("ERROR!!! set_aileron_angle CALLED WHEN control_servos_disabled\n");
        return;
    }
    printf("Aileron angle: %d\n", angle);
}

void set_elevator_angle(int angle) {
    if (control_servos_disabled) {
        printf("ERROR!!! set_elevator_angle CALLED WHEN control_servos_disabled\n");
        return;
    }
    printf("Elevator angle: %d\n", angle);
}

// Motor

int motor_disabled = 0;

int setup_motor() {
    motor_disabled = 0;
    printf("Motor setup correctly\n");
}

void disable_motor() {
    printf("Motor disabled\n");
    motor_disabled = 1;
}

void set_throttle(uint8_t throttle) {
    if (motor_disabled) {
        printf("ERROR!!! set_throttle CALLED WHEN motor_disabled\n");
        return;
    }
    printf("Motor throttle: %d\n", throttle);
}

// IMU

int imu_disabled = 0;

int setup_imu() {
    imu_disabled = 0;
    return 0;
}

void read_imu(float imu_data[6]) {
    if (imu_disabled) {
        printf("ERROR!! read_imu CALLED WHEN imu_disabled\n");
        return;
    }
    imu_data[0] = 0.0f;
    imu_data[1] = 0.0f;
    imu_data[2] = 0.0f;
    imu_data[3] = 0.0f;
    imu_data[4] = 0.0f;
    imu_data[5] = 0.0f;
}

float read_imu_temp() {
    if (imu_disabled) {
        printf("ERROR!! read_imu_temp CALLED WHEN imu_disabled\n");
        return 0.0f;
    }
    return 0.0f;
}

void disable_imu() {
    imu_disabled = 1;
    printf("IMU disabled\n");
}
