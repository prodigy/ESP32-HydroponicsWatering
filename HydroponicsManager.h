#ifndef HYDROPONICSMANAGER_H
#define HYDROPONICSMANAGER_H

#include "TickObjectList.h"
#include "TickObject.hpp"
#include "WaterDetector.h"
#include "LightDiode.h"
#include "Relay.hpp"
#include "State.hpp"

static const byte STATE_FILL = 0;
static const byte STATE_DRAIN = 1;
static const byte STATE_IDLE = 2;

// Declare state, resolves circular reference issue.
class State;

class HydroponicsManager : public TickObject {
  public:
    HydroponicsManager();
    ~HydroponicsManager();

    LightDiode* light_diode;
    WaterDetector* flood_complete_detector;
    WaterDetector* drain_complete_detector;
    Relay* pump;

    TickObjectList* getObjectList();
    void tick(unsigned int diff);
    void setState(byte newState);
  private:
    TickObjectList* objectList;

    byte state = -1;
    State* stateObj = nullptr;
    bool isInitialized = false;
    void initialize();
};

#endif
