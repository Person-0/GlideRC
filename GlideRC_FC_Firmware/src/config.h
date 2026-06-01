#ifndef ON_FC
    #define uart0 0
    #define uart1 1
    #define spi0 0
    #define spi1 1
#endif

/*
 Channel
 Control Numbers
*/
#define CHANNEL_MOTOR 0
#define CHANNEL_AILERON 1
#define CHANNEL_ELEVATOR 2

/*
 PIN DEFINITIONS
*/

// Receiver (SBUS protocol)
#define SBUS_PIN 1 // uart rx
#define SBUS_UART_ID uart0

// IMU
#define IMU_SDO 4
#define IMU_NCS 5
#define IMU_SCL 6
#define IMU_SDA 7
#define IMU_SPI spi0

// Motor
#define ESC_SIGNAL_PIN 8
#define ESC_FREQ 50
#define ESC_MIN 1000
#define ESC_MAX 2000
#define ESC_LIMIT 0.25f

// Control surfaces
#define AILERON_SERVO_PIN 9
#define ELEVATOR_SERVO_PIN 10
#define SERVO_FREQ 50
#define SERVO_MIN 1000
#define SERVO_MAX 2000
#define SERVO_MID 1500
