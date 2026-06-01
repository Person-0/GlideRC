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

// Motor
#define ESC_SIGNAL_PIN 8
#define ESC_FREQ 50
#define ESC_MIN 1000
#define ESC_MAX 2000

// Control surfaces
#define AILERON_SERVO_PIN 9
#define ELEVATOR_SERVO_PIN 10
#define SERVO_FREQ 50
#define SERVO_MIN 1000
#define SERVO_MAX 2000
#define SERVO_MID 1500