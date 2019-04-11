#include "DHa4988.h"

float degree_table[5] = { 1.8, 0.9, 0.45, 0.225, 0.1125 };

DHa4988::DHa4988(uint8_t enable, uint8_t dir, uint8_t step, uint8_t ms1, uint8_t ms2, uint8_t ms3) {
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

void DHa4988::setMode(uint8_t mode) {
  if (mode < 0 || mode > 5 && mode != 4) {
    Serial.println("[DHa4988] Invalid mode supplied to driver! Ignoring...");
    return;
  }
  _mode = mode;
  _updateIO();
}

void DHa4988::setDirection(uint8_t direction) {
  if (direction < 0 || direction > 1) {
    Serial.println("[DHa4988] Invalid direction supplied to driver! Ignoring...");
    return;
  }
  _direction = direction;
  _updateIO();
}

void DHa4988::setSpeed(uint8_t speed) {
  if (speed < 1 || speed > 100) {
    Serial.println("[DHa4988] Invalid direction supplied to driver! Ignoring...");
    return;
  }
  _speed = speed;
}

void DHa4988::step(float degrees) {
  float stepAngle = degree_table[_mode];
  float steps = degrees/stepAngle;

  if (fmod(steps, 1) != 0) {
    Serial.println("[DHa4988] Inputted step degrees is not compabitible with current mode! Rounding to closest step...");
  }

  uint8_t newSteps = (int)round(steps);
  uint8_t delay = map(100 - _speed, 0, 100, 300, 4000);

  for (uint8_t i = 0; i < newSteps; i++) {
    digitalWrite(_step, HIGH);
    delayMicroseconds(delay);
    digitalWrite(_step, LOW);
    delayMicroseconds(delay);
  }
}

void DHa4988::enable() {
  if (_enabled) return;
  _enabled = true;
  _updateIO();
}

void DHa4988::disable() {
  if (!_enabled) return;
  _enabled = false;
  _updateIO();
}

void DHa4988::attachButton(uint8_t button, float degrees, uint8_t edge, uint8_t mode, bool autoDisable) {
  if (fmod(degrees/degree_table[_mode], 1) != 0) {
    Serial.println("[DHa4988] Inputted step degrees is not compabitible with current mode! Disregarding button attach...");
    return;
  }

  if (_buttonEdge < 0 || _buttonEdge > 1) {
    Serial.println("[DHa4988] Invalid button edge supplied to driver! Disregarding button attach...");
    return;
  }

  if (_mode < 0 || _mode > 1) {
    Serial.println("[DHa4988] Invalid button mode supplied to driver! Disregarding button attach...");
    return;
  }

  _button = button;
  _buttonStep = degrees;
  _buttonEdge = edge;
  _buttonMode = mode;
  _prevButtonState = mode;
  _autodisable = autoDisable;

  if (_autodisable) {
    disable();
  }

  pinMode(_button, INPUT);
}

void DHa4988::checkButtonPress() {
  if (!_button) return;

  uint8_t currentState = digitalRead(_button);
  if (_buttonMode == D_BUTTON_PULLUP) {
    if (_buttonEdge == D_RISING) {
      if (_prevButtonState == 0 && currentState == 1) {
        if (_autodisable) {
          enable();
        }
        delay(100);
        step(_buttonStep);
      }
    } else {
      if (currentState == 0) {
        if (_autodisable) {
          enable();
        }
        delay(100);
        step(_buttonStep);
      }
    }
  } else {
    if (_buttonEdge == D_RISING) {
      if (currentState == 1) {
        if (_autodisable) {
          enable();
        }
        delay(100);
        step(_buttonStep);
      }
    } else {
      if (_prevButtonState == 1 && currentState == 0) {
        if (_autodisable) {
          enable();
        }
        delay(100);
        step(_buttonStep);
      }
    }
  }
  if (_autodisable) {
    disable();
  }
  _prevButtonState = currentState;
}

void DHa4988::_updateIO() {
  digitalWrite(_enable, !_enabled);
  digitalWrite(_dir, _direction);
  digitalWrite(_ms1, _mode & (1 << 0));
  digitalWrite(_ms2, _mode & (1 << 1));
  digitalWrite(_ms3, _mode & (1 << 2));
}
