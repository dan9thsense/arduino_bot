/*
ArduinoBlue example code to demonstrate the features of the app.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;


// MOTOR PARAMETERS

// we'll use this knowing that our speeds are between 0,100
const int MINIMUM_MOTOR_SPEED = 25;

int leftBTVelocity = 0; // negative to go backward and the value to between -100, 100 for speed
int rightBTVelocity = 0; // negative to go backward and the value to between -100, 100 for speed


//********************** HC-08 Bluetooth stuff *****************************/

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;

int LED_PIN = 13;
bool ledOn = false;
int ledBtnId = 100;


int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;
int shiftedThrottle, shiftedSteering, mappedSpeed, reducedSpeed;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor
//********************** end Bluetooth stuff *****************************/

// Setup code runs once after program starts.
void setup() {
    // Start serial communications.
    // The baud rate must be the same for both the serial and the bluetooth.
    Serial.begin(BAUD_RATE);

    pinMode( LED_PIN, OUTPUT );

    /***** BLUETOOTH STUFF **********/
    bluetooth.begin(BAUD_RATE);
    delay(100);

    Serial.println("setup complete");
    /***** END BLUETOOTH STUFF **********/
}

// Put your main code here, to run repeatedly:
void loop() {
  customControls();
  navigationControls();
}

void dansFunction()
{
    Serial.print("\tleftBTVelocity: "); Serial.print(leftBTVelocity);
    Serial.print("\trightBTVelocity: "); Serial.println(rightBTVelocity);  
}

/*********** BLUETOOTH STUFF ****************************/
void customControls() {
     // ID of the button pressed pressed.
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent.
    // calls until text data is sent again.
    String str = phone.getText(); 
        // ID of the slider moved.
    sliderId = phone.getSliderId();

    // Slider value goes from 0 to 200.
    sliderVal = phone.getSliderVal();

    // Display button data whenever its pressed.
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);

        if ( button == ledBtnId )
        {
          if ( ledOn )
            digitalWrite( LED_PIN, LOW );
          else
            digitalWrite( LED_PIN, HIGH );          

          ledOn = !ledOn;
        }
    }

    // Display slider data when slider moves
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
    }

    // If a text from the phone was sent print it to the serial monitor
    if (str != "") {
        Serial.println(str);
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("send: ");
        String str = Serial.readString();
        phone.sendMessage(str); // phone.sendMessage(str) sends the text to the phone.
        Serial.print(str);
        Serial.write('\n');
    }
}

void navigationControls() {
  
    // Throttle and steering values go from 0 to 99.
    // When throttle and steering values are at 99/2 = 49, the joystick is at center.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    shiftedThrottle = throttle - 49;
    shiftedSteering = steering - 49;
  
    // THROTTLE AND STEERING CONTROL
    // throttle values are [0,99]
    // throttle values after subtracting 49:
    //     50 = max forward throttle
    //     0 = no throttole
    //     -49 = max reverse throttle
    // steering values after subtracting 49:
    //     50 = max right
    //     0 = straight
    //     -49 = max left
    if (shiftedThrottle == 0) {
      // If throttle is zero, don't move.
      leftBTVelocity = 0;
      rightBTVelocity = 0;
    }
    else
    {  
      // Map speeds from [0,50] to [0,100] range.
      mappedSpeed = map(abs(shiftedThrottle), 0, 50, MINIMUM_MOTOR_SPEED, 100);
      // Map steering to PWM range.
      reducedSpeed = map(abs(shiftedSteering), 0, 50, mappedSpeed, MINIMUM_MOTOR_SPEED);
    
      // If backwards
      if (shiftedThrottle < 0) {
        // make speeds negative
        mappedSpeed = -mappedSpeed;
        reducedSpeed = -reducedSpeed;
      }
    
      if (shiftedSteering > 0) {
        // Turn Right: reduce right motor speed
        leftBTVelocity = mappedSpeed;
        rightBTVelocity= reducedSpeed;
      }
      else {
        // Turn Left: reduce left motor speed
        leftBTVelocity  = reducedSpeed;
        rightBTVelocity = mappedSpeed;
      }    
    }

    // Display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {

        Serial.print("throttle: "); Serial.print(throttle);
        Serial.print("\tsteering: "); Serial.print(steering);
        Serial.print("\tmappedSpeed: "); Serial.print(mappedSpeed);
        Serial.print("\treducedSpeed: "); Serial.print(reducedSpeed);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    // Set motor speeds
    dansFunction();

}

/*********** END BLUETOOTH STUFF ****************************/

