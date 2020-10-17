#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"

// ToDo: implement state machine to handle "all off" gracefully (without sending lots of "n")

namespace BusDashboard {

    class Dashboard;

    class Shifter : public ButtonListener
    {
    public:
        bool setCurrentState(const uint8_t button, const bool state);
        void registerWith(ButtonHandler &bh);
        Shifter();
        void begin();

    protected:
    private:
        enum class State
        {
            Undefined,
            Off,
            D,
            D_leaving,
            R,
            R_leaving,
            N,
            N_leaving
        };

        enum class Index
        {
            D = 0,
            R = 1,
            N = 2
        };
        static const uint8_t MAX_INDEX = (uint8_t)Index::N;
        State _currentState = State::Undefined;
        bool _state[MAX_INDEX + 1];
        bool _charToSend[MAX_INDEX + 1];
        bool _initDone = false;
        unsigned long _lastBlinkToggle = 0;

        Shifter(const Shifter &) = delete;
        Shifter &operator=(const Shifter &) = delete;
    };
} // namespace BusDashboard
