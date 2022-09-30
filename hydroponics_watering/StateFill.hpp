#ifndef STATEFILL_H
#define STATEFILL_H

#include "State.hpp"
#include "LightDiode.h"

#define FILL_TIME_WAIT_CHECK 15000
#define FILL_TIME_WAIT_BEFORE_DRAIN 480000

static const byte FILL_STATE_BEGIN = 0;
static const byte FILL_STATE_PUMP = 1;
static const byte FILL_STATE_WAIT = 2;
static const byte FILL_STATE_FINISHED = 3;

class StateFill : public State {
  public:
    StateFill(HydroponicsManager* manager) : State(manager) {
      Serial.println("Initializing StateFill...");
      this->interval = new TimeIntervalGuard(FILL_TIME_WAIT_CHECK);
      int state1 = digitalRead(16);
      int state2 = digitalRead(17);
      Serial.printf("pin state: %d, %d", state1, state2);
      Serial.println();
      this->setState(FILL_STATE_BEGIN);
    }

    ~StateFill() {
      delete this->interval;
    }

    void tick(unsigned int diff) {
      switch(this->fillState) {
        case FILL_STATE_BEGIN:
          this->handleStateBegin();
          break;
        case FILL_STATE_PUMP:
          this->handleStatePump();
          break;
        case FILL_STATE_WAIT:
          this->handleStateWait(diff);
          break;
        case FILL_STATE_FINISHED:
          this->handleStateFinished(diff);
          break;
      }
    }

  private:
    TimeIntervalGuard* interval;
    bool isFillDetected = false;
    byte fillState = -1;

    void setState(byte state) {
      Serial.printf("Fill state: %d", state);
      Serial.println();
      this->fillState = state;
      this->initState(state);
    }

    void initState(byte state) {
      switch(state) {
        case FILL_STATE_BEGIN:
          this->manager->valve->setActive(true);
        case FILL_STATE_PUMP:
          Serial.println("initializing begin/pump");
          this->manager->light_diode->setMode(LightDiode::MODE_STATIC);
          this->manager->light_diode->set(LightDiode::COLOUR_BLUE);
          break;
        case FILL_STATE_WAIT:
          Serial.println("initializing wait");
          this->interval->resetSetInterval(FILL_TIME_WAIT_CHECK);
          break;
        case FILL_STATE_FINISHED:
          Serial.println("initializing finished");
          this->manager->light_diode->setMode(LightDiode::MODE_BLINKING);
          this->manager->light_diode->set(LightDiode::COLOUR_TEAL);
          this->interval->resetSetInterval(FILL_TIME_WAIT_BEFORE_DRAIN);
          break;
        default:
          break;
      }
    }

    void handleStateBegin() {
      Serial.println("enabling valve");
      this->manager->valve->setActive(true);
      this->setState(FILL_STATE_PUMP);
    }

    void handleStatePump() {
      Serial.println("enabling pump");
      this->manager->pump->setActive(true);
      this->setState(FILL_STATE_WAIT);
    }

    void handleStateWait(unsigned int diff) {
      // Serial.println("wait...");
      bool filledDetected = this->manager->flood_complete_detector->getIsWatered();
      bool filledDetectedPreviously = this->isFillDetected;
      this->isFillDetected = filledDetected;
      
      auto pump = this->manager->pump;
      if (!filledDetected) {
        if(this->interval->updateCheckElapsed(diff) && !pump->getIsActive()) {
          this->setState(FILL_STATE_PUMP);
        }
        return;
      }

      if(pump->getIsActive()) {
        Serial.println("disabling pump");
        pump->setActive(false);
        this->interval->reset();
        this->manager->light_diode->setMode(LightDiode::MODE_BLINKING);
        this->manager->light_diode->set(LightDiode::COLOUR_BLUE);
      } else {
        if(this->interval->updateCheckElapsed(diff)) {
          if (filledDetectedPreviously) {
            this->setState(FILL_STATE_FINISHED);
            return;
          }
        }
      }
    }

    void handleStateFinished(unsigned int diff) {
      if(this->interval->updateCheckElapsed(diff)) {
        Serial.println("Finished Filling!");
        this->interval->setEnabled(false);
        this->manager->valve->setActive(false);
        this->manager->setState(STATE_IDLE);
      }
    }
};

#endif
