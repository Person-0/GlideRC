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

// State variables
bool EXCEPTION_OCCURED = false;
float imu_data[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

void sleep_forever() {
    printf("sleeping forever.\n");
    while (1) sleep_ms(1000);
}

void disable_components() {
    disable_motor();
    disable_control_servos();
    disable_imu();
}

void on_error_cleanup() {
    EXCEPTION_OCCURED = true;
    disable_components();
    sleep_forever();
}

// Recieves data from the receiver
void receiver_channel_callback(uint8_t channel, uint16_t value) {

    #ifndef ON_FC
        printf("Channel Callback received %d value for channel %d\n", value, channel);
    #endif

    switch(channel) {

        case CHANNEL_MOTOR:
            set_throttle(value * 100 / 2047);
            break;
        
        case CHANNEL_AILERON:
            set_aileron_angle(value * 180 / 2047);
            break;

        case CHANNEL_ELEVATOR:
            set_elevator_angle(value * 180 / 2047);
            break;
    }

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
    if (setup_sbus_uart() > 0) {
        printf("FATAL Receiver/SBUS error, Exiting.\n");
        on_error_cleanup();
    }
    register_channel_callback(receiver_channel_callback);

    // Control Surfaces
    if (setup_control_servos() > 0) {
        printf("FATAL Control Servos error, Exiting.\n");
        on_error_cleanup();
    }

    // Motor
    if (setup_motor() > 0) {
        printf("FATAL Motor error, Exiting.\n");
        on_error_cleanup();
    }

    // IMU
    if (setup_imu() > 0) {
        printf("FATAL IMU error, Exiting.\n");
        on_error_cleanup();
    }

    // main loop
    while (!EXCEPTION_OCCURED) {
        read_imu(imu_data);
        #ifdef ON_FC
            // === CURRENTLY ONLY LOGGING (phase 1) ===
            printf("imu data read\n");
            printf(
                "Acceleration in G     X = %10.4f,  Y = %10.4f,  Z = %10.4f\n",
                imu_data[0],
                imu_data[1],
                imu_data[2]
            );
            printf("Gyroscope in Deg/s    X = %10.4f,  Y = %10.4f,  Z = %10.4f\n",
                imu_data[3],
                imu_data[4],
                imu_data[5]
            );
        #endif
        sleep_ms(100);
    }

    printf("EXCEPTION_OCCURED, main loop exited\n");
    on_error_cleanup();
}
