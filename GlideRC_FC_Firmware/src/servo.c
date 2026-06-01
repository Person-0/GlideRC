#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "servo.h"

void setup_servo(uint8_t pin, int freq, int pos) {
    printf("setting up servo on pin %d\n", pin);
    #ifdef ON_FC
        gpio_set_function(pin, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(pin);
        uint32_t clock_speed =  clock_get_hz(clk_sys);
        float div = clock_speed / 1000000.0f;
        pwm_set_clkdiv(slice_num, div);
        uint16_t wrap = (1000000.0f / freq) - 1;
        pwm_set_wrap(slice_num, wrap);
        pwm_set_gpio_level(pin, pos);
        pwm_set_enabled(slice_num, true);
        printf("servo setup on pin %d complete\n", pin);
    #else
        printf("servo at pin %d was not setup as not ON_FC\n", pin);
    #endif
}

/*
 @param angle -> [0, 180]
*/
void servo_move(uint8_t pin, int angle, int min, int max) {
    if (angle < 0 || angle > 180) {
        printf(
            "servo_move: angle out of bounds. pin: %d, angle: %d\n",
            pin,
            angle
        );
        return;
    }
    #ifdef ON_FC
        pwm_set_gpio_level(pin, min + (angle * (max - min)) / 180);
    #endif
}

void disable_servo(uint8_t pin) {
    printf("disable_servo: servo at Pin %d has been disabled\n", pin);
    #ifdef ON_FC
        pwm_set_enabled(pwm_gpio_to_slice_num(pin), false);
    #endif
}