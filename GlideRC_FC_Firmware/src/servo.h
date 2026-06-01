#ifndef SERVO_HNDLR
#define SERVO_HNDLR

#ifndef ON_FC
    #include <stdint.h>
#endif

static void setup_servo(uint8_t pin, int freq, int pos);
static void servo_move(uint8_t pin, int angle, int min, int max);
static void disable_servo(uint8_t pin);

#endif