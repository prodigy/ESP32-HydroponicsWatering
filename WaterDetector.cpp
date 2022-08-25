#include <Arduino.h>
#include "WaterDetector.h"
#include "TimeIntervalGuard.h"

WaterDetector::WaterDetector(byte pin, float threshold) {
  this->pin = pin;
  this->threshold = threshold;
  this->interval_guard = new TimeIntervalGuard(this->interval);
}

WaterDetector::~WaterDetector() {
  if (this->interval_guard != nullptr) {
    delete this->interval_guard;
  }
}

bool WaterDetector::getIsWatered() {
  return this->isWatered;
}

void WaterDetector::tick(unsigned int diff) {
  if(!this->interval_guard->updateCheckElapsed(diff)) {
    return;
  }
  this->interval_guard->reset();

  float reading = 0;
  for(byte i = 0; i < MEASURE_SAMPLE_SIZE; i++) {
    reading += touchRead(this->pin);
  }
  reading /= MEASURE_SAMPLE_SIZE;

  if (reading <= this->threshold) {
    this->isWatered = true;
  } else {
    this->isWatered = false;
  }
}
