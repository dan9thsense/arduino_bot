#include <Arduino.h>
#include <ArduinoBlue.h>
#include "little_bot_defines.h"
//#include "pololu_simple_motor_serial2.h"
#include "bluetooth_little_bot.h"
//#include "little_bot_instantiates.h"
//Motor left_motor(POLOLU_LEFT_MOTOR_RESET_PIN, POLOLU_LEFT_MOTOR_ERROR_PIN);
//Motor right_motor(POLOLU_RIGHT_MOTOR_RESET_PIN, POLOLU_RIGHT_MOTOR_ERROR_PIN);

// speed should be a number from -3200 to 3200
void setMotorSpeedLeft(int speed) {
  if (speed < 0) {
    Serial2.write(0x86); // motor reverse command
    speed = -speed; // make speed positive
  }
  else
  {
    Serial2.write(0x85); // motor forward command
  }
    Serial2.write(speed & 0x1F);
    Serial2.write(speed >> 5);
}

// speed should be a number from -3200 to 3200
void setMotorSpeedRight(int speed) {
  if (speed < 0) {
    Serial3.write(0x86); // motor reverse command
    speed = -speed; // make speed positive
  }
  else
  {
    Serial3.write(0x85); // motor forward command
  }
    Serial3.write(speed & 0x1F);
    Serial3.write(speed >> 5);
}

void setup() {
  
  Serial.begin(115200);
  Serial.print("test print5\n"); 

  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);  
  delay(5);

  // briefly reset SMC when Arduino starts up (optional)
  //pinMode(resetPin, OUTPUT);
  //digitalWrite(resetPin, LOW); // reset SMC
  //delay(1); // wait 1 ms
  pinMode(POLOLU_RIGHT_MOTOR_RESET_PIN, INPUT); // let SMC run again
  // must wait at least 1 ms after reset before transmitting
  delay(5);
  // this lets us read the state of the SMC ERR pin (optional)
  pinMode(POLOLU_RIGHT_MOTOR_ERROR_PIN, INPUT); 


  Serial2.write(0xAA); // send baud-indicator byte
  Serial3.write(0xAA); // send baud-indicator byte  
  delay(5);
  Serial2.write(0x83);
  Serial3.write(0x83);  

}
  
// the loop function runs over and over again forever
void loop() {
  customControls();
  navigationControls();
  dansFunction();  

  setMotorSpeedLeft(leftBTVelocity*16);
  setMotorSpeedRight(rightBTVelocity*16);  
    
  delay(10);
   

}
