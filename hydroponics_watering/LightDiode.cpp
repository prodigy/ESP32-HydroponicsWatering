#include <Arduino.h>
#include "LightDiode.h"
#include "TimeIntervalGuard.hpp"

LightDiode::LightDiode() {
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
    
  this->set(COLOUR_OFF);
}

LightDiode::~LightDiode() {
  this->clearIntervalGuard();
}

void LightDiode::clearIntervalGuard() {
  if(this->interval_guard != nullptr) {
    delete this->interval_guard;
    this->interval_guard = nullptr;
  }
}

void LightDiode::set(byte colour) {
  this->desired_state = colour;
}

void LightDiode::setMode(byte mode) {
  this->mode = mode;

  this->clearIntervalGuard();

  switch (this->mode)
  {
  case MODE_BLINKING:
    this->interval_guard = new TimeIntervalGuard(this->blink_interval);
    break;  
  default:
    break;
  }
}

void LightDiode::setBlinkInterval(byte interval) {
  if (interval < BLINK_INTERVAL_MIN) {
    interval = BLINK_INTERVAL_MIN;
  }
  if (interval > BLINK_INTERVAL_MAX) {
    interval = BLINK_INTERVAL_MAX;
  }
  this->blink_interval = interval;

  if (this->mode == MODE_BLINKING) {
    this->interval_guard->setInterval(this->blink_interval);
  }
}

void LightDiode::tick(unsigned int diff) {
  switch(this->mode) {
    case MODE_STATIC:
      this->tickModeStatic();
      break;
    case MODE_BLINKING:
      this->tickModeBlinking(diff);
      break;
  }
}

void LightDiode::tickModeStatic() {
  if (this->current_state != this->desired_state) {
    this->writeState(this->desired_state);
  }
}

void LightDiode::tickModeBlinking(unsigned int diff) {
  if (!this->interval_guard->updateCheckElapsed(diff)) {
    return;
  }
  this->interval_guard->reset();

  if (this->current_state == COLOUR_OFF) {
    this->writeState(this->desired_state);
  } else {
    this->writeState(COLOUR_OFF);
  }
}

void LightDiode::writeState(byte colour) {
  bool red = (byte)((colour & COLOUR_RED) > 0) ? LOW : HIGH;
  bool green = (byte)((colour & COLOUR_GREEN) > 0) ? LOW : HIGH;
  bool blue = (byte)((colour & COLOUR_BLUE) > 0) ? LOW : HIGH;
  digitalWrite(RGB_RED, red);
  digitalWrite(RGB_GREEN, green);
  digitalWrite(RGB_BLUE, blue);
  this->current_state = colour;
}
