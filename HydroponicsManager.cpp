#include <Arduino.h>
#include "HydroponicsManager.h"
#include "constants.h"
#include "StateFill.hpp"
#include "StateDrain.hpp"
#include "StateIdle.hpp"

HydroponicsManager::HydroponicsManager() {
  this->light_diode = new LightDiode();
  this->flood_complete_detector = new WaterDetector(PIN_SENSOR_FLOOD_COMPLETE, FLOOD_THRESHOLD);
  this->drain_complete_detector = new WaterDetector(PIN_SENSOR_DRAIN_COMPLETE, DRAIN_THRESHOLD);
  this->pump = new Relay(PIN_PUMP);

  byte objectCount = 3;
  this->objectList = new TickObjectList(objectCount);
  this->objectList->add(light_diode);
  this->objectList->add(flood_complete_detector);
  this->objectList->add(drain_complete_detector);

  this->light_diode->setMode(LightDiode::MODE_BLINKING);
  this->light_diode->set(LightDiode::COLOUR_YELLOW);

  this->initialize();
}

HydroponicsManager::~HydroponicsManager() {
  delete this->objectList;
  delete this->light_diode;
  delete this->flood_complete_detector;
  delete this->drain_complete_detector;
}

TickObjectList* HydroponicsManager::getObjectList() {
  return this->objectList;
}

void HydroponicsManager::tick(unsigned int diff) {
  for(byte idx = 0; idx < this->objectList->getCount(); idx++) {
    TickObject* obj = this->objectList->get(idx);
    if (obj != nullptr && obj->isEnabled()) {
      obj->tick(diff);
    }
  }

  if(this->stateObj != nullptr) {
    this->stateObj->tick(diff);
  }
}

void HydroponicsManager::initialize() {
  Serial.println("initialize()");
  this->isInitialized = true;

  // When the drain detector says it's watered, let the system drain. Otherwise fill.
  if (this->drain_complete_detector->getIsWatered()) {
    Serial.println("drain water detected, draining...");
    this->setState(STATE_DRAIN);
  } else {
    Serial.println("filling...");
    this->setState(STATE_FILL);
  }
}

void HydroponicsManager::setState(byte newState) {
  if(this->state == newState) {
    return;
  }

  this->state = newState;

  if (this->stateObj != nullptr) {
    delete this->stateObj;
  }

  Serial.printf("Going into state %d...", this->state);
  Serial.println();
  switch (this->state)
  {
    case STATE_FILL:
      this->stateObj = new StateFill(this);
      break;
    case STATE_DRAIN:
      this->stateObj = new StateDrain(this);
      break;
    case STATE_IDLE:
      this->stateObj = new StateIdle(this);
      break;
    default:
      Serial.printf("Unknown state: %d", this->state);
      Serial.println();
      break;
  }
}
