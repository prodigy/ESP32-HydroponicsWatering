#ifndef TIMEINTERVALGUARD_H
#define TIMEINTERVALGUARD_H

class TimeIntervalGuard {
  public:
    TimeIntervalGuard(unsigned int interval);
    void reset();
    bool updateCheckElapsed(unsigned int diff);
    void setInterval(unsigned int interval);
  private:
    unsigned int time_elapsed = 0;
    unsigned int interval;
};

#endif
