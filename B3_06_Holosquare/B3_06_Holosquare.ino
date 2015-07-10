/* B3_10_HoloSquare.ino
   This is a test sketch for autonomous driving mode of
   the three-wheeled drive Bread Board Bot (BBbot, B^3)

   Arduino: Arduino Mega 256 v3 Clone
   Motor Shield: Adafruit assembled Motor Shield for Arduino v2
   ---->  http://www.adafruit.com/products/1438

   Programmer: Dave Eslinger; December 2, 2014
   Revisions: 2015, May 25:   Changed for new motor configuration. DLE
              2015, June 12:  Changed into B3_ code style for GoSciTech 2015. DLE
              2015, July 9: Name change, cleaned up and additional comments added. DLE
*/
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <math.h>
#include <breadboardbot.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Define Constants
const float cos30sin60 = sqrt(3.0) / 2.0; // cos(30 deg) = sin(60 deg), need for wheel
// vector calcs.
const byte LEFT_PR_PIN = A15;    // Define light sensor pins for left
const byte RIGHT_PR_PIN = A14;   // and right sides
const bool PHOTOTAXIS = true;    // GOes towards light (true) or away (false)
const byte BASESPEED = 165;       // Base speed.  Light values will reduce this to turn
const bool SIMPLE_LOGIC = false; // Use a simple or more complex approach

// Define 'ports' for motors.
const byte LEFT_MOTOR_PORT = 3;
const byte RIGHT_MOTOR_PORT = 1;
const byte BACK_MOTOR_PORT = 4;
// Create pointers to motor control objects
Adafruit_DCMotor *motorLeft = AFMS.getMotor(LEFT_MOTOR_PORT);
Adafruit_DCMotor *motorRight = AFMS.getMotor(RIGHT_MOTOR_PORT);
Adafruit_DCMotor *motorBack = AFMS.getMotor(BACK_MOTOR_PORT);

// Define global variables
float direction;       // Velocity Vector Angle (DEGREES) from forward to drive
float magnitude;       // Magnitude (0-100) of movement vector in given direction
float duration;        // Duration to drive at given velocity vector

byte motorLeftdir;     // Clockwise or Counter clockwise for the 3 wheels
byte motorBackdir;
byte motorRightdir;

void setup(void) {
  AFMS.begin();  // create with the default frequency 1.6KHz
  // Turn off all motors
  motorLeft->run(RELEASE);
  motorBack->run(RELEASE);
  motorRight->run(RELEASE);
}
void loop(void) {
  // Section for taking commands from Serial Input
  int leftLight = analogRead(LEFT_PR_PIN);
  int rightLight = analogRead(RIGHT_PR_PIN);
  Serial.print(leftLight);           // We can only write ONE thing
  Serial.print(" = left, right = "); // at a time, therefore it takes
  Serial.println(rightLight);        // three statemens to get our output.
if (SIMPLE_LOGIC) {
    motorLeft->setSpeed(map(rightLight, 0, 500, 50, 255));
    motorRight->setSpeed(map(leftLight, 0, 500, 50, 255));
    byte direction;
    if (PHOTOTAXIS) {
      direction = FORWARD;
    }
    else {
      direction = BACKWARD;
    }
    motorLeft->run(direction);
    motorRight->run(direction);
  }
  else {
    /* Find the direction of the strongest light by taking the difference
        between the readings */
    int deltaLight = abs(leftLight - rightLight);
    deltaLight = constrain(deltaLight, 0, BASESPEED);  // Keep that limit between 0 and 50

    byte leftSpeed = BASESPEED;
    byte rightSpeed = BASESPEED;


    if (PHOTOTAXIS) {
      if (leftLight - rightLight > 0) {       // The LEFT side is the brightest,
        leftSpeed = leftSpeed - deltaLight;   // slow down the LEFT side to turn towards it
      }
      else if (leftLight - rightLight < 0 ) { // The RIGHT side is the brightest,
        rightSpeed = rightSpeed - deltaLight-10; // slow down the RIGHT side to turn towards it
      }
    }
    else {
      if (leftLight - rightLight > 0) {       // The LEFT side is the brightest,
        rightSpeed = rightSpeed - deltaLight; // slow down the RIGHT side to turn away
      }
      else if (leftLight - rightLight < 0 ) { // The Right side is the brightest,
        leftSpeed = leftSpeed - deltaLight-10;   // slow down the LEFT side to turn away
      }
    }
    motorLeft->setSpeed(leftSpeed+10);
    motorRight->setSpeed(rightSpeed);
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
  }

  }


