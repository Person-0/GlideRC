#include <stdio.h>
#include <stdint.h>
#include "../src/hardware.h"

void chnl_cb(uint8_t channel, uint16_t value) {
    printf("Channel Callback received %d value for channel %d\n", value, channel);
}

int main() {
    printf("hello world\n");
    register_channel_callback(chnl_cb);

    uint8_t sbus_bytes[25];

    sbus_bytes[0] = 0x0F; // start byte

    // 22 data bytes
    for (int i = 1; i < 23; i++) {
        // 0xff is 11111111 (8bits, every bit is 1)
        // so it means all channels will be 11111111111 (11 bits, every bit is 1)
        // which in decimal is 2047
        sbus_bytes[i] = 0xFF;
    }

    sbus_bytes[23] = 0x00; // flags
    sbus_bytes[24] = 0x00; // end byte

    inject_sbus_bytes(sbus_bytes);

    printf("DONE\n");

    return 0;
}
