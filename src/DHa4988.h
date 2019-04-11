/*****************************************************************************
**  File name: DylanStepper.h
**  Description: Driver for a4988 driver board
**  Notes:
**  Author(s): Humphrey, Dylan
**  Created: 4/11/2019
**  Last Modified: 4/11/2019
**  Changes:
******************************************************************************/

#ifndef DSTEPPER_H
#define DSTEPPER_H

#include "Arduino.h"

#define FULL_STEP 0 // 1.8 degrees per step
#define HALF_STEP 1 // 0.9 degrees per step
#define QUARTER_STEP 2 // 0.45 degrees per step
#define EIGHTH_STEP 3 // 0.225 degres per step
#define SIXTEENTH_STEP 5 // 0.1125 degrees per step

#define CW 1
#define CCW 0

#define D_RISING 0
#define D_FALLING 1

#define D_BUTTON_PULLDOWN 0
#define D_BUTTON_PULLUP 1

class DHa4988 {
public:
  /** Create a DHa4988 instance
   *
   *  @param enable The pin which is connected to the enable input on the stepper driver
   *  @param dir The pin connected to the dir input on the stepper driver
   *  @param step The pin connected to the step input on the stepper driver
   *  @param ms1 The pin connected to the ms1 input on the stepper driver
   *  @param ms2 The pin connected to the ms2 input on the stepper driver
   *  @param ms3 The pin connected to the ms3 input on the stepper driver
   *
   * @note Initializes all specified pins to outputs. Defaults the mode to
   * FULL_STEP, direction to clockwise, enabled to true, and the speed to 50%.
   */
  DHa4988(uint8_t enable, uint8_t dir, uint8_t step, uint8_t ms1, uint8_t ms2, uint8_t ms3);


  /** Set the step mode
   *
   *  @param mode The mode you wish to put the stepper motor into
   *
   *  @note This mode must be a valid mode which are specified in the defines
   *  above.
   */
  void setMode(uint8_t mode);

  /** Set the direction
   *
   *  @param direction A valid direction to set (clockwise or counter clockwise)
   */
  void setDirection(uint8_t direction);

  /** Set the speed
   *
   *  @param speed The perecent speed to set. Must be between 0 and 100 inclusive
   */
  void setSpeed(uint8_t speed);

  /** Step the motor in the set direction the set amount of degrees
   *
   *  @param degrees A float representing how many degrees to step the motor.
   *
   *  @note The degrees to step must be a multiple of the current mode. ie. if The
   *  current mode is HALF_STEP, degrees must be a multiple of 0.9. If not, then
   *  the amount of steps will be rounded to the nearest integer. ie. If you
   *  want to step 2.0 degrees with HALF_STEP, it will only step 2 times for a total
   *  of 1.8 degrees.
   */
  void step(float degrees);

  /** Enables the motor for use
   *
   *  @note Only necessary to call if the motor was previously disabled as the motor
   *  is enabled by default.
   */
  void enable();

  /** Disables the motor
   *
   *  @note This diables all outputs such that the motor cannot be controlled.
   *  However in this disabled state it is still possible to change things around
   *  As all the inputs and internal transalator logic remain active
   */
  void disable();

  /** Attach a button such that it steps the set amount of degrees on the edge defined
   *
   *  @param button The digital pin which the button is attached to
   *  @param degrees The amount of degrees to turn on a button press
   *  @param edge The edge to actually do the steps on. Must be either D_RISING or D_FALLING.
   *  @param mode The pinmode for the button. Either D_BUTTON_PULLUP or D_BUTTON_PULLDOWN
   *  @param autoDisable Disable the motor while waiting for button presses
   *
   *  @note Setting the pinmode does NOT configure internal pull ups or pull downs.
   *  Those must be added externally in hardware. The pinmode is only used to
   *  correctly detect rising and falling edges.
   */
  void attachButton(uint8_t button, float degrees, uint8_t edge, uint8_t mode, bool autoDisable);

  /** Checks for a button press based on the specified parameters in attachButton
   *
   *  @note attachButton must be called before this can be used. This function also
   *  must be called repeatedly in a loop to detect button changes
   */
  void checkButtonPress();

private:
  bool _enabled;
  bool _autodisable;
  uint8_t _direction;
  uint8_t _enable;
  uint8_t _dir;
  uint8_t _step;
  uint8_t _ms1;
  uint8_t _ms2;
  uint8_t _ms3;
  uint8_t _mode;
  uint8_t _speed;
  uint8_t _button;
  float _buttonStep;
  uint8_t _buttonEdge;
  uint8_t _buttonMode;
  uint8_t _prevButtonState;

  void _updateIO();
};

#endif
