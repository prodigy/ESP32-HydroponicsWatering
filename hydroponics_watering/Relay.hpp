#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
  public:
    Relay(uint8_t pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
      setActive(this->isActive);
    }
    void setActive(bool state) {
      this->isActive = state;
      digitalWrite(this->pin, state ? HIGH : LOW);
    }
    bool getIsActive() {
      return this->isActive;
    }
  private:
    uint8_t pin;
    bool isActive = false;
};

#endif
