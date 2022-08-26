#include <Arduino.h>
#include "TickObjectList.h"
#include "TickObject.hpp"

TickObjectList::TickObjectList(byte maxSize) {
  this->max_size = maxSize;
  this->count = 0;
  TickObject** arr = new TickObject*[this->max_size];
  this->objects = arr;
}

void TickObjectList::add(TickObject* obj) {
  if(this->count < this->max_size) {
    this->objects[count++] = obj;
  }
}

TickObject* TickObjectList::get(byte index) {
  if (index < 0 || index >= this->max_size) {
    return nullptr;
  }
  return this->objects[index];
}

byte TickObjectList::getCount() {
  return this->count;
}
