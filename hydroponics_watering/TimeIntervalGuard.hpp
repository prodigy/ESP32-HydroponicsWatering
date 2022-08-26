#ifndef TIMEINTERVALGUARD_H
#define TIMEINTERVALGUARD_H

class TimeIntervalGuard {
  public:
    TimeIntervalGuard(unsigned int interval) {
      this->setInterval(interval);
    }
    void reset() {
      this->time_elapsed = 0;
      this->is_enabled = true;
    }
    void resetSetInterval(unsigned int interval) {
      this->reset();
      this->setInterval(interval);
    }
    bool updateCheckElapsed(unsigned int diff) {
      this->time_elapsed += diff;
      return is_enabled && this->time_elapsed >= this->interval;
    }
    void setInterval(unsigned int interval) {
      this->interval = interval;
    }
    void setEnabled(bool enabled) {
      this->is_enabled = enabled;
    }
  private:
    unsigned int time_elapsed = 0;
    unsigned int interval;
    bool is_enabled = true;
};

#endif
