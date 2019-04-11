#include <DHa4988.h>

// define our output pins
const int enable = 2;
const int ms1 = 3;
const int ms2 = 4;
const int ms3 = 5;
const int step = 6;
const int dir = 7;

// define our button input
const int button = 8;

DHa4988 stepper(enable, dir, step, ms1, ms2, ms3);

void setup() {
  // configure the stepper to be in half step mode
  stepper.setMode(HALF_STEP);

  // configure the speed to 75% of max
  stepper.setSpeed(75);

  // configure the direciton to counter clockwise
  stepper.setDirection(CCW);

  // attach a button to turn the motor 180 degrees on each press
  // the button has an external pulldown resistor
  // when the button is pressed, the stepper will step upon release
  // autodisable the motor when not in use
  stepper.attachButton(button, 180.0, D_FALLING, D_BUTTON_PULLDOWN, true);
}

void loop() {
  // all we do is check the button repeatedly
  stepper.checkButtonPress();
}
