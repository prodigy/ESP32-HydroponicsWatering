#include "constants.h"

#include "TickObject.hpp"
#include "TickObjectList.h"
#include "TimeIntervalGuard.h"
#include "WaterDetector.h"
#include "LightDiode.h"
#include "Relay.hpp"
#include "State.hpp"
#include "StateFill.hpp"
#include "HydroponicsManager.h"

unsigned int lastTick = 0;
HydroponicsManager* hydroponics_manager;

void setup() {
  Serial.begin(115200);
  Serial.println("startup");
  hydroponics_manager = new HydroponicsManager();
}

void loop() {
  unsigned int diff = getDiff();
  hydroponics_manager->tick(diff);  
  delay(100);
}

unsigned int getDiff() {
  unsigned int ms = millis();
  unsigned int diff = ms - lastTick;
  lastTick = ms;
  return diff;
}
