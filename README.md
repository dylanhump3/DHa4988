# DylanStepper

A simple, customizable driver for the a4988 stepper motor board for Arduino.

## Example
```
const int enable = 2;
const int ms1 = 3;
const int ms2 = 4;
const int ms3 = 5;
const int step = 6;
const int dir = 7;

// initalize the stepper class
DylanStepper stepper(enable, dir, step, ms1, ms2, ms3);

void setup() {
  // change mode to Half step (0.9 degrees per step)
  stepper.setMode(HALF_STEP);
}

void loop() {
  // step 90 degrees
  stepper.step(90.0);
  delay(2000);
}
```

## Functions

`void setMode(int mode)`

Specify the desired mode. Valid modes are:

| Mode           	| Degrees per step 	| Default 	|
| :-------------:	|:----------------:	|:-------:	|
| FULL_STEP      	| 1.8              	| YES     	|
| HALF_STEP      	| 0.9              	| NO      	|
| QUARTER_STEP   	| 0.45             	| NO      	|
| EIGHTH_STEP    	| 0.225            	| NO      	|
| SIXTEENTH_STEP 	| 0.1125           	| NO      	|

`void setDirection(int direction)`

Set the desired direction. Valid directions are:

| Direction     	|  Default 	|
| :-------------:	| :-------:	|
| CW      	      | YES     	|
| CCW      	      |  NO      	|

`void setSpeed(int speed)`

Set the desired speed. Speed is given in percentage of the maximum speed. Thus
these values are between 0 and 100 inclusive. Max speed corresponds to 1667
steps per second. The table below shows how rpm varies with selected mode at
maximum speed.

| Mode           	| Degrees per second | RPM  	|
| :-------------:	|:----------------:	 |:------:|
| FULL_STEP      	| 3000               | 500    |
| HALF_STEP      	| 1500             	 | 250    |
| QUARTER_STEP   	| 750             	 | 125   	|
| EIGHTH_STEP    	| 375             	 | 62.5   |
| SIXTEENTH_STEP 	| 187.5           	 | 31.25  |

Note speeds will most likely be slower due to the time it takes for `digitalWrite`

`void step(float degrees)`

Step the motor the specified amount of degrees.

The degrees must be a multiple of the current mode. ie. if The
current mode is HALF_STEP, degrees must be a multiple of 0.9. If not, then
the amount of steps will be rounded to the nearest integer. ie. If you
want to step 2.0 degrees with HALF_STEP, it will only step 2 times for a total
of 1.8 degrees. To step 2 degrees, you would could select EIGHTH_STEP which would
step 9 times resulting in 2.025 degrees.

`void enable()`

Enable the motor for use if it was previously disabled. No need to call this
otherwise as it is enabled by default.

`void disable()`

Disable the motor. This disables the outputs controlling the motor. The inputs
and internal logic still remain operational so it is still possible to change
the mode and direction of the motor.

`void attachButton(int button, float degrees, int edge, int mode)`

Attach a button such that upon its press and the specified edge and mode, the stepper
will turn the specified degrees. Valid edges are `D_RISING` and `D_FALLING` which
specify the rising or falling edge of the button. Valid modes are `D_BUTTON_PULLUP`
and `D_BUTTON_PULLDOWN` which specify if a pullup or pulldown resistor are connected
externally. Note that this function does not configure any internal pullup/down resistors.
As with `step()`, the `degrees` must be a multiple of the current mode, or the button
attach will be ignored.

Example use:
```
// Our button is connected to digital pin 8
// We want to spin 180.0 on the rising edge of our button
// Based on the pulldown configuration the button will be LOW unless pressed
// The stepper will spin the instant the button is pressed as this is the rising edge
stepper.attachButton(D8, 180.0, D_RISING, D_BUTTON_PULLDOWN);
```

Note that this function has a 100ms delay for debouncing the button.

`void checkButtonPress()`

This function must be called in a loop in order to check for the conditions
specified in `attachButton()`. `attachButton()` must be called before this
function is ever called.

## TODO

- [ ] Use pin interrupts if specified input allows it for selected board
- [ ] Configure internal pullup/down resistors if the selected board allows for it
