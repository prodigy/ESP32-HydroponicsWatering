#ifndef STATEIDLE_H
#define STATEIDLE_H

#include "State.hpp"
#include "LightDiode.h"

static const unsigned long IDLE_TIME_SLEEP=3600*1000*1000; // 1 hour

class StateIdle : public State {
  public:
    StateIdle(HydroponicsManager* manager) : State(manager) {
      auto led = this->manager->light_diode;
      led->setMode(LightDiode::MODE_STATIC);
      led->set(LightDiode::COLOUR_YELLOW);
    }
    ~StateIdle() {
    }
    void tick(unsigned int diff) {
      unsigned long operatingTime = micros();
      unsigned long timeSleep = (operatingTime > IDLE_TIME_SLEEP) ? 1 : IDLE_TIME_SLEEP - operatingTime;
      Serial.printf("going to sleep for %lu (%lu/%x) seconds... (operating time was: %lu)", timeSleep/1000/1000, timeSleep, timeSleep, operatingTime/1000/1000);
      esp_sleep_enable_timer_wakeup(timeSleep);
      esp_deep_sleep_start();
      // End of program. Board goes into sleep mode.
    }
};

#endif
