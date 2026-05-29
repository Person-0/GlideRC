#include <stdio.h>
#include "pico/stdlib.h"

#include "src/hardware.h"

int main() {
    stdio_init_all();

    printf("> GlideRC_FC_Firmware\n");

    printf("waiting for 1.5s...\n");
    sleep_ms(1500);

    printf("Setting up SBUS UART port...\n");
    setup_sbus_uart();

    while (1) {}
}
