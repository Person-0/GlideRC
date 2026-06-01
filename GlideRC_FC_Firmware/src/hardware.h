#ifndef FC_HARDWARE
#define FC_HARDWARE

#ifndef ON_FC
    #include <stdint.h>
#endif

void setup_sbus_uart();
void inject_sbus_byte(uint8_t data);
void register_channel_callback(void (*callback)(uint8_t, uint16_t));

void setup_control_servos();
void disable_control_servos();
void set_aileron_angle(int angle);
void set_elevator_angle(int angle);

void setup_motor();
void disable_motor();
void set_throttle(uint8_t throttle);

#endif