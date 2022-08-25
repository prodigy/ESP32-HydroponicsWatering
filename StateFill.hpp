#ifndef STATEFILL_H
#define STATEFILL_H

#include "State.hpp"
#include "LightDiode.h"

#define FILL_TIME_WAIT_CHECK 5000
#define FILL_MAX_RETRY 5

class StateFill : public State {
  public:
    StateFill(HydroponicsManager* manager) : State(manager) {
      Serial.println("Initializing StateFill...");
      auto led = this->manager->light_diode;
      led->setMode(LightDiode::MODE_BLINKING);
      led->set(LightDiode::COLOUR_BLUE);
      this->interval = new TimeIntervalGuard(FILL_TIME_WAIT_CHECK);
      this->manager->pump->setActive(true);
    }
    ~StateFill() {
      delete this->interval;
    }
    void tick(unsigned int diff) {
      if (this->isWaitingEnsureFlood) {
        if(this->interval->updateCheckElapsed(diff)) {
          bool newPumpState = !this->manager->pump->getIsActive();
          this->manager->pump->setActive(newPumpState);
          this->interval->reset();
          if(newPumpState == false) {
            this->isWaitingEnsureFlood = false;
            this->isFilled = false;
          }
        }
      } else {
        bool filledDetected = this->manager->flood_complete_detector->getIsWatered();
        if (filledDetected) {
          if (this->fillRetryCount >= FILL_MAX_RETRY || this->isFilled) {
            Serial.println("Fill complete, setting state to DRAIN.");
            this->manager->setState(STATE_DRAIN);
          } else {
            Serial.println("Disabling pump, waiting...");
            ++fillRetryCount;
            this->manager->pump->setActive(false);
            this->isWaitingEnsureFlood = true;
            this->interval->reset();
          }
        } else {
          this->manager->pump->setActive(true);
        }
        this->isFilled = filledDetected;
      }
    }
  private:
    TimeIntervalGuard* interval;
    bool isFilled = false;
    bool isWaitingEnsureFlood = false;
    byte fillRetryCount = 0;
};

#endif
