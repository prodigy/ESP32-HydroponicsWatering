#include <Arduino.h>
#include "TimeIntervalGuard.h"

TimeIntervalGuard::TimeIntervalGuard(unsigned int interval) {
    this->interval = interval;
}

void TimeIntervalGuard::reset() {
    this->time_elapsed = 0;
}

bool TimeIntervalGuard::updateCheckElapsed(unsigned int diff) {
    this->time_elapsed += diff;
    return this->time_elapsed >= this->interval;
}

void TimeIntervalGuard::setInterval(unsigned int interval) {
    this->interval = interval;
}
