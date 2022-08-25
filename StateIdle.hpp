#ifndef STATEIDLE_H
#define STATEIDLE_H

#include "State.hpp"
#include "LightDiode.h"

#define IDLE_TIME_WAIT 10800*1000

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
      Serial.println("going to sleep...");
      esp_sleep_enable_timer_wakeup((IDLE_TIME_WAIT - millis())*1000);
      esp_deep_sleep_start();
      // End of program. Board goes into sleep mode.
    }
};

#endif
