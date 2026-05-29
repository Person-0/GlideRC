#ifndef FC_HARDWARE
#define FC_HARDWARE

#ifdef ON_FC
    #include "pico/stdlib.h"
    #include "hardware/uart.h"
    #include "hardware/irq.h"
#else
    #include <stdint.h>
#endif

void setup_sbus_uart();
void inject_sbus_byte(uint8_t data);

#endif