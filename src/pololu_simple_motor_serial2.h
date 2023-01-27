#ifndef POLOLU_SIMPLE_MOTOR_H_
#define POLOLU_SIMPLE_MOTOR_H_

// some variable IDs
#define ERROR_STATUS 0
#define LIMIT_STATUS 3
#define TARGET_SPEED 20
#define INPUT_VOLTAGE 23
#define TEMPERATURE 24

// some motor limit IDs
#define FORWARD_ACCELERATION 5
#define REVERSE_ACCELERATION 9
#define DECELERATION 2

class Motor {
  public:
    Motor(int resetPin, int errPin) {
      //serial_port_ = serIn;
      //Serial2.begin(9600);
      //Serial2.begin(9600);
      // the Simple Motor Controller must be running for at least 1 ms
      // before we try to send serial data, so we delay here for 5 ms
      delay(5);

      // briefly reset SMC when Arduino starts up (optional)
      //pinMode(resetPin, OUTPUT);
      //digitalWrite(resetPin, LOW); // reset SMC
      //delay(1); // wait 1 ms
      pinMode(resetPin, INPUT); // let SMC run again
      // must wait at least 1 ms after reset before transmitting
      delay(5);
      // this lets us read the state of the SMC ERR pin (optional)
      pinMode(errPin, INPUT);

      // if the Simple Motor Controller has automatic baud detection
      // enabled, we first need to send it the byte 0xAA (170 in decimal)
      // so that it can learn the baud rate
      Serial2.write(0xAA); // send baud-indicator byte
      
      // next we need to send the Exit Safe Start command, which
      // clears the safe-start violation and lets the motor run
      //exitSafeStart(); // clear the safe-start violation and let the motor run
    }
    
    // required to allow motors to move
    // must be called when controller restarts and after any error
    void exitSafeStart() {
      Serial2.write(0x83);
    }
    
    // speed should be a number from -3200 to 3200
    void setMotorSpeed(int speed) {
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

    // read a serial byte (returns -1 if nothing received after the timeout expires)
    int readByte() {
      char c;
      if (Serial2.readBytes(&c, 1) == 0){ return -1; }
      return (byte)c;
    }

    unsigned char setMotorLimit(unsigned char limitID, unsigned int limitValue) {
      Serial2.write(0xA2);
      Serial2.write(limitID);
      Serial2.write(limitValue & 0x7F);
      Serial2.write(limitValue >> 7);
      return readByte();
    }
    
    // returns the specified variable as an unsigned integer.
    // if the requested variable is signed, the value returned by this function
    // should be typecast as an int.
    unsigned int getVariable(unsigned char variableID) {
      Serial2.write(0xA1);
      Serial2.write(variableID);
      return readByte() + 256 * readByte();
    }

};

#endif // POLOLU_SIMPLE_MOTOR_H_