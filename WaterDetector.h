#ifndef WATERDETECTOR_H
#define WATERDETECTOR_H

#include "TickObject.hpp"
#include "TimeIntervalGuard.h"

#define MEASURE_SAMPLE_SIZE 8
#define MEASURE_INTERVAL_DEFAULT 500

class WaterDetector : public TickObject {
  public:
    WaterDetector(byte pin, float threshold);
    ~WaterDetector();
    void tick(unsigned int diff);
    bool getIsWatered();
  private:
    byte pin;
    float threshold;
    bool isWatered = false;
    unsigned int interval = MEASURE_INTERVAL_DEFAULT;
    unsigned int timeSinceLastMeasurement = 0;
    TimeIntervalGuard* interval_guard = nullptr;
};

#endif
