#ifndef STATE_H
#define STATE_H

#include "TickObject.hpp"
#include "HydroponicsManager.h"

// Declare HydroponicsManager, resolves circular reference issue.
class HydroponicsManager;

class State : public TickObject {
  public:
    State(HydroponicsManager* manager) {
      this->manager = manager;
    }
  protected:
    HydroponicsManager* manager;
};

#endif
