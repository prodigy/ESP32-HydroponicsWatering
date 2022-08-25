#ifndef TICKOBJECT_H
#define TICKOBJECT_H

class TickObject {
  public:
    bool isEnabled() {
      return this->is_enabled;
    }
    void setEnabled(bool state) {
      this->is_enabled = state;
    }
    virtual void tick(unsigned int diff) = 0;
  private:
    bool is_enabled = true;
};

#endif
