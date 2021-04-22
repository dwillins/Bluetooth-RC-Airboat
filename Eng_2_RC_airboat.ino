#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h>

const unsigned long BAUD_RATE = 9600;

// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;

const int led = 12;

int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

Servo rudder;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor

// Setup code runs once after program starts.
void setup() {
    // Start serial communications.
    // The baud rate must be the same for both the serial and the bluetooth.
    Serial.begin(BAUD_RATE);
    bluetooth.begin(BAUD_RATE);
    delay(100);

    Serial.println("setup complete");

    pinMode(led, OUTPUT);
    rudder.attach(6);
}

// Put your main code here, to run repeatedly:
void loop() {
    // ID of the button pressed pressed.
    button = phone.getButton();

    // Throttle and steering values go from 0 to 99.
    // When throttle and steering values are at 99/2 = 49, the joystick is at center.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // Display button data whenever its pressed.
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);
        digitalWrite(led, !digitalRead(led));
    }


    // Display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {
        Serial.print("Throttle: "); Serial.print(throttle); Serial.print("\tSteering: "); Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    steer(steering);
    
}

void steer(int degrees) {
  if (degrees < 100){
    rudder.write((degrees-6)/1.5);
    Serial.println((degrees-6)/1.5);
  }
}
