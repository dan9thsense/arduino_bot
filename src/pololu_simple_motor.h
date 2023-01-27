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
    Motor(HardwareSerial *serIn, int resetPin, int errPin) {
      serial_port_ = serIn;
      serial_port_->begin(9600);
      // the Simple Motor Controller must be running for at least 1 ms
      // before we try to send serial data, so we delay here for 5 ms
      delay(5);

      // briefly reset SMC when Arduino starts up (optional)
      pinMode(resetPin, OUTPUT);
      digitalWrite(resetPin, LOW); // reset SMC
      delay(1); // wait 1 ms
      pinMode(resetPin, INPUT); // let SMC run again
      // must wait at least 1 ms after reset before transmitting
      delay(5);
      // this lets us read the state of the SMC ERR pin (optional)
      pinMode(errPin, INPUT);

      // if the Simple Motor Controller has automatic baud detection
      // enabled, we first need to send it the byte 0xAA (170 in decimal)
      // so that it can learn the baud rate
      serial_port_->write(0xAA); // send baud-indicator byte
      
      // next we need to send the Exit Safe Start command, which
      // clears the safe-start violation and lets the motor run
      exitSafeStart(); // clear the safe-start violation and let the motor run
    }
    
    // required to allow motors to move
    // must be called when controller restarts and after any error
    void exitSafeStart() {
      serial_port_->write(0x83);
    }
    
    // speed should be a number from -3200 to 3200
    void setMotorSpeed(int speed) {
      if (speed < 0) {
        serial_port_->write(0x86); // motor reverse command
        speed = -speed; // make speed positive
      }
      else
      {
        serial_port_->write(0x85); // motor forward command
      }
        serial_port_->write(speed & 0x1F);
        serial_port_->write(speed >> 5);
    }

    // read a serial byte (returns -1 if nothing received after the timeout expires)
    int readByte() {
      char c;
      if (serial_port_->readBytes(&c, 1) == 0){ return -1; }
      return (byte)c;
    }

    unsigned char setMotorLimit(unsigned char limitID, unsigned int limitValue) {
      serial_port_->write(0xA2);
      serial_port_->write(limitID);
      serial_port_->write(limitValue & 0x7F);
      serial_port_->write(limitValue >> 7);
      return readByte();
    }
    
    // returns the specified variable as an unsigned integer.
    // if the requested variable is signed, the value returned by this function
    // should be typecast as an int.
    unsigned int getVariable(unsigned char variableID) {
      serial_port_->write(0xA1);
      serial_port_->write(variableID);
      return readByte() + 256 * readByte();
    }
    
  private:
    HardwareSerial *serial_port_;
};

#endif // POLOLU_SIMPLE_MOTOR_H_
