#include <stdio.h>
#include <stdint.h>

#include "../src/hardware.h"

void chnl_cb(uint8_t channel, uint16_t value) {
    printf("Channel Callback received %d value for channel %d\n", value, channel);
}

int main() {
    printf("hello world\n");
    register_channel_callback(chnl_cb);

    inject_sbus_byte(0x0F); // start byte

    // 22 data bytes
    for (int i = 0; i < 22; i++) {
        // 0xff is 11111111 (8bits, every bit is 1)
        // so it means all channels will be 11111111111 (11 bits, every bit is 1)
        // which in decimal is 2047
        inject_sbus_byte(0xFF);
    }

    inject_sbus_byte(0x00); // flags
    inject_sbus_byte(0x00); // end byte

    printf("DONE\n");

    return 0;
}