#include <Arduino.h> 
#include <Adafruit_MotorShield.h> // Need to get appropriate library if new arduino install
#include "hardware_operations.h" 

#define PIN_LED_FAIL   17
#define PIN_CANDY_DISPENSE_DETECT 27 // Beambreaker collector port
#define PIN_USER_EXTRACTION_DETECT 7 // Beambreaker collector port
#define MOTOR_PRIMARY_DISPENSE_SPEED 20 // 0 TO 255 Valid
#define MOTOR_ROTATION_PER_DISPENSE (MOTOR_ROTATION_FULL_360_DEG / 4)
#define MOTOR_ROTATE_TRUE 0X44 // Capital "D"

// Setup instance of motor stepper
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object with the default I2C address
Adafruit_StepperMotor *MotorPrimaryDispense = AFMS.getStepper(516, 2); // Connect a stepper motor with 516 steps per revolution to motor port #2 (M3 and M4)

// -------------------------------------------------------------------------------------------- //
void SetFailLed(bool setLedOn) {
  if (setLedOn) {
    digitalWrite(PIN_LED_FAIL, HIGH);
  } else {
    digitalWrite(PIN_LED_FAIL, LOW);
  }
}
// -------------------------------------------------------------------------------------------- //
void SetUpHardware () {
  //configure the led
  pinMode(PIN_LED_FAIL, OUTPUT);

  // Configuring the motor
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    SetFailLed(true);
    while (1);
  }
  MotorPrimaryDispense->setSpeed(MOTOR_PRIMARY_DISPENSE_SPEED); // configure speed
  
  // configuring beam breakers
  pinMode(PIN_CANDY_DISPENSE_DETECT, INPUT_PULLUP);  // define beambreaker port mode
  pinMode(PIN_USER_EXTRACTION_DETECT, INPUT_PULLUP);  // define beambreaker port mode
}
// -------------------------------------------------------------------------------------------- //
void MotorMovePrimaryDispense (int StepsToMove) { // 512 == full rotation //old geizer command

  MotorPrimaryDispense->step(StepsToMove, FORWARD, DOUBLE); 
}
// -------------------------------------------------------------------------------------------- //
void ControlMotor (char parameter) {
 if (parameter == 0x44) {
 MotorMovePrimaryDispense(MOTOR_ROTATION_PER_DISPENSE);
 
 }
}
// -------------------------------------------------------------------------------------------- //
bool IsCandyDispensed () {
    if (digitalRead(PIN_CANDY_DISPENSE_DETECT) == LOW) { 
      return true;
    } else {
      return false;
    }
}
// -------------------------------------------------------------------------------------------- //
bool IsCandyTaken () {
    if (digitalRead(PIN_USER_EXTRACTION_DETECT) == LOW) { 
      return true;
    } else {
      return false;
    }
}
// -------------------------------------------------------------------------------------------- //
