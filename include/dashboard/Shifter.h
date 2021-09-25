#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard {

    class Shifter : public ButtonListener
    {
    public:
        void setCurrentState(const uint8_t button, const bool state, const bool prev_state) override;
        void registerWith(ButtonHandler &bh) override;
        Shifter();
        void begin() override {};

    protected:

    private:
        enum class State : uint8_t
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
