#ifndef LITTLE_BOTS_DEFINES
#define LITTLE_BOTS_DEFINES

// motor controllers
#define LEFT 0
#define RIGHT 1
//HardwareSerial pololu_motor_serial_port = POLOLU_MOTOR_SERIAL_PORT;
#define POLOLU_LEFT_MOTOR_RESET_PIN 5 // pin 5 connects to left pololu simple motor controller nRST
#define POLOLU_RIGHT_MOTOR_RESET_PIN 6 // pin 6 connects to right pololu simple motor controller nRST
#define POLOLU_LEFT_MOTOR_ERROR_PIN 7 // pin 7 connects to left pololu simple motor controller ERR
#define POLOLU_RIGHT_MOTOR_ERROR_PIN 8 // pin 8 connects to right pololu simple motor controller ERR


// motor driver device numbers (written onto the driver board EEPROM)
//#define LEFT_MOTOR_NUMBER 14     // don't know this one-- usb connector is blocked so it is hard to check
//#define RIGHT_MOTOR_NUMBER 13


#endif //LITTLE_BOTS_DEFINES
