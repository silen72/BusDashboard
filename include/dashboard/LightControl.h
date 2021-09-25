#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard
{
    class ButtonHandler;

    class LightControl : public ButtonListener
    {
    public:
        LightControl();
        void setCurrentState(const uint8_t button, const bool state, const bool prev_state) override;
        void registerWith(ButtonHandler &bh) override;
        void begin() override {};
        bool isLit() { return _dashboardLit; }

    protected:
    private:
        static uint8_t const WAIT_MAX_MS_IN_LEAVING_STATE_MS = 50;
        static uint8_t const WAIT_FOR_OFF_BLINK_DELAY_MS = 250;
        enum class State : uint8_t
        {
            Undefined,    // before initialization, never reached afterwards
            Wait_for_off, // this state indicates that the knob has to be set to off to sync it with the OMSI bus state
            Off,          // lighting control knob is in position "off"; this cannot be measured directly because "all off" can mean both "really off" and "between two on positions"
            A1,           // lighting control knob is in position "A1"
            A1_leaving,   // A1 has changed to LOW but no other knob position is measured HIGH yet
            A2,           // lighting control knob is in position "A2"
            A2_leaving,   // A2 has changed to LOW but no other knob position is measured HIGH yet
            I1,           // lighting control knob is in position "I1"
            I1_leaving,   // I1 has changed to LOW but no other knob position is measured HIGH yet
            I2,           // lighting control knob is in position "I2"
            I2_leaving    // I2 has changed to LOW but no other knob position is measured HIGH yet
        };
        State _currentState = State::Undefined;
        uint64_t _leavingTS = 0;
        bool _dashboardLit = false;

        LightControl(const LightControl &) = delete;
        LightControl &operator=(const LightControl &) = delete;
    };
} // namespace BusDashboard
