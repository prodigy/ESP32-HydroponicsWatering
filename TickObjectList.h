#ifndef TICKOBJECTLIST_H
#define TICKOBJECTLIST_H

#include "TickObject.hpp"

class TickObjectList {
  public:
    TickObjectList(byte maxSize);
    void add(TickObject* obj);
    TickObject* get(byte index);
    byte getCount();
  private:
    byte count;
    byte max_size;
    TickObject** objects;
};

#endif
