#include "DylanStepper.h"

float degree_table[5] = { 1.8, 0.9, 0.45, 0.225, 0.1125 };

DylanStepper::DylanStepper(int enable, int dir, int step, int ms1, int ms2, int ms3) {
  _enable = enable;
  _dir = dir;
  _step = step;
  _ms1 = ms1;
  _ms2 = ms2;
  _ms3 = ms3;
  _mode = FULL_STEP;
  _enabled = true;
  _speed = 50;
  _direction = CW;

  pinMode(_enable, OUTPUT);
  pinMode(_dir, OUTPUT);
  pinMode(_step, OUTPUT);
  pinMode(_ms1, OUTPUT);
  pinMode(_ms2, OUTPUT);
  pinMode(_ms3, OUTPUT);

  _updateIO();
}

void DylanStepper::setMode(int mode) {
  if (mode < 0 || mode > 5 && mode != 4) {
    Serial.println("[Dylan Stepper] Invalid mode supplied to driver! Ignoring...");
    return;
  }
  _mode = mode;
  _updateIO();
}

void DylanStepper::setDirection(int direction) {
  if (direction < 0 || direction > 1) {
    Serial.println("[Dylan Stepper] Invalid direction supplied to driver! Ignoring...");
    return;
  }
  _direction = direction;
  _updateIO();
}

void DylanStepper::setSpeed(int speed) {
  if (speed < 1 || speed > 100) {
    Serial.println("[Dylan Stepper] Invalid direction supplied to driver! Ignoring...");
    return;
  }
  _speed = speed;
}

void DylanStepper::step(float degrees) {
  float stepAngle = degree_table[_mode];
  float steps = degrees/stepAngle;

  if (fmod(steps, 1) != 0) {
    Serial.println("[Dylan Stepper] Inputted step degrees is not compabitible with current mode! Rounding to closest step...");
  }

  int newSteps = (int)steps;
  int delay = map(100 - _speed, 0, 100, 300, 4000);

  for (int i = 0; i < newSteps; i++) {
    digitalWrite(_step, HIGH);
    delayMicroseconds(delay);
    digitalWrite(_step, LOW);
    delayMicroseconds(delay);
  }
}

void DylanStepper::enable() {
  if (_enabled) return;
  _enabled = true;
  _updateIO();
}

void DylanStepper::disable() {
  if (!_enabled) return;
  _enabled = false;
  _updateIO();
}

void DylanStepper::attachButton(int button, float degrees, int edge, int mode) {
  if (fmod(degrees/degree_table[_mode], 1) != 0) {
    Serial.println("[Dylan Stepper] Inputted step degrees is not compabitible with current mode! Disregarding button attach...");
    return;
  }

  if (_buttonEdge < 0 || _buttonEdge > 1) {
    Serial.println("[Dylan Stepper] Invalid button edge supplied to driver! Disregarding button attach...");
    return;
  }

  if (_mode < 0 || _mode > 1) {
    Serial.println("[Dylan Stepper] Invalid button mode supplied to driver! Disregarding button attach...");
    return;
  }

  _button = button;
  _buttonStep = degrees;
  _buttonEdge = edge;
  _buttonMode = mode;
  _prevButtonState = mode;

  pinMode(_button, INPUT);
}

void DylanStepper::checkButtonPress() {
  if (!_button) return;

  int currentState = digitalRead(_button);
  if (_buttonMode == D_BUTTON_PULLUP) {
    if (_buttonEdge == D_RISING) {
      if (_prevButtonState == 0 && currentState == 1) {
        delay(100);
        step(_buttonStep);
      }
    } else {
      if (currentState == 0) {
        delay(100);
        step(_buttonStep);
      }
    }
  } else {
    if (_buttonEdge == D_RISING) {
      if (currentState == 1) {
        delay(100);
        step(_buttonStep);
      }
    } else {
      if (_prevButtonState == 1 && currentState == 0) {
        delay(100);
        step(_buttonStep);
      }
    }
  }
  _prevButtonState = currentState;
}

void DylanStepper::_updateIO() {
  digitalWrite(_enable, !_enabled);
  digitalWrite(_dir, _direction);
  digitalWrite(_ms1, _mode & (1 << 0));
  digitalWrite(_ms2, _mode & (1 << 1));
  digitalWrite(_ms3, _mode & (1 << 2));
}
