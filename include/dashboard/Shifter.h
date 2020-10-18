#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard {

    class Shifter : public ButtonListener
    {
    public:
        void setCurrentState(const uint8_t button, const bool state, const bool prev_state);
        void registerWith(ButtonHandler &bh);
        Shifter();
        void begin() {};

    protected:

    private:
        enum class State
        {
            Undefined,
            R,
            N,
            D
        };
        State _state = State::Undefined;
        bool _initDone = false;
        unsigned long _lastBlinkToggle = 0;

        Shifter(const Shifter &) = delete;
        Shifter &operator=(const Shifter &) = delete;
    };
} // namespace BusDashboard
