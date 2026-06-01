#include <stdio.h>
#include <stdbool.h>
#ifdef ON_FC
    #include "pico/stdlib.h"
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif

#include "src/config.h"
#include "src/hardware.h"

void sleep_forever() {
    printf("sleeping forever.\n");
    while (1) sleep_ms(1000);
}

void disable_components() {
    disable_motor();
    disable_control_servos();
}

// State variables
bool EXCEPTION_OCCURED = false;
float imu_data[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

// Recieves data from the receiver
void reciever_channel_callback(uint8_t channel, uint16_t value) {
    printf("Channel Callback received %d value for channel %d\n", value, channel);
}

int main() {
    #ifdef ON_FC
        stdio_init_all();
    #endif

    printf("> GlideRC_FC_Firmware\n");

    printf("waiting for 1.5s...\n");
    sleep_ms(1500);

    // Receiver
    printf("Setting up SBUS UART port...\n");
    setup_sbus_uart();
    register_channel_callback(reciever_channel_callback);

    // Control Surfaces
    setup_control_servos();

    // Motor
    setup_motor();

    // IMU
    if (setup_imu() > 0) {
        printf("FATAL IMU error, Disabling motor and servos.\n");
        EXCEPTION_OCCURED = true;
    }

    // main loop
    while (!EXCEPTION_OCCURED) {
        read_imu(imu_data);
        printf("imu data read\n");
        sleep_ms(100);
    }

    printf("EXCEPTION_OCCURED, main loop exited\n");
    disable_components();
    sleep_forever();
}
