#ifndef FC_HARDWARE
#define FC_HARDWARE

#ifndef ON_FC
    #include <stdint.h>
#endif

void setup_sbus_uart();
void inject_sbus_byte(uint8_t data);

#endif