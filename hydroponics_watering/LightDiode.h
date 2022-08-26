#ifndef LIGHTDIODE_H
#define LIGHTDIODE_H

#include "TickObject.hpp"
#include "TimeIntervalGuard.hpp"

#define RGB_RED 4
#define RGB_GREEN 2
#define RGB_BLUE 0

#define BLINK_INTERVAL_MIN 100
#define BLINK_INTERVAL_MAX 5000
#define BLINK_INTERVAL_DEFAULT 500

class LightDiode : public TickObject {
  public:
    LightDiode();
    ~LightDiode();

    void set(byte colour);
    void setMode(byte mode);
    void setBlinkInterval(byte interval);
    void tick(unsigned int diff);

    static const byte COLOUR_RED = 1;
    static const byte COLOUR_GREEN = 2;
    static const byte COLOUR_BLUE = 4;
    static const byte COLOUR_YELLOW = COLOUR_RED | COLOUR_GREEN;
    static const byte COLOUR_PURPLE = COLOUR_RED | COLOUR_BLUE;
    static const byte COLOUR_TEAL = COLOUR_GREEN | COLOUR_BLUE;
    static const byte COLOUR_WHITE = COLOUR_RED | COLOUR_GREEN | COLOUR_BLUE;
    static const byte COLOUR_OFF = 0;

    static const byte MODE_STATIC = 0;
    static const byte MODE_BLINKING = 1;
  private:
    byte current_state = COLOUR_OFF;
    byte desired_state = COLOUR_OFF;
    byte mode;

    void writeState(byte colour);

    void tickModeStatic();

    unsigned int blink_interval = BLINK_INTERVAL_DEFAULT;
    void tickModeBlinking(unsigned int diff);
    TimeIntervalGuard* interval_guard = nullptr;
    void clearIntervalGuard();
};

#endif
