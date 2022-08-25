#ifndef STATEDRAIN_H
#define STATEDRAIN_H

#include "State.hpp"
#include "LightDiode.h"

#define FILL_TIME_WAIT_CHECK 10000
#define FILL_MAX_RETRY 1

class StateDrain : public State {
  public:
    StateDrain(HydroponicsManager* manager) : State(manager) {
      Serial.println("Initializing StateDrain...");
      auto led = this->manager->light_diode;
      led->setMode(LightDiode::MODE_BLINKING);
      led->set(LightDiode::COLOUR_PURPLE);
      this->manager->pump->setActive(false);
    }
    ~StateDrain() {
    }
    void tick(unsigned int diff) {
        bool isDrained = !this->manager->drain_complete_detector->getIsWatered();
        if (isDrained) {
          Serial.println("is drained!");
          this->manager->setState(STATE_IDLE);
        }
    }
};

#endif
