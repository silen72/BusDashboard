#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard
{
    class Dashboard;
    class ButtonHandler;

    class LightControl : public ButtonListener
    {
    public:
        LightControl();
        bool setCurrentState(const uint8_t button, const bool state);
        void registerWith(ButtonHandler &bh);
        void begin();
        bool isLit() { return _dashboardLit; }

    protected:
    private:
        static uint8_t const WAIT_MAX_MS_IN_LEAVING_STATE_MS = 50;
        static uint8_t const WAIT_FOR_OFF_BLINK_DELAY_MS = 250;
        enum class Index
        {
            A1,
            A2,
            I1,
            I2
        };
        static uint8_t const MaxIndex = (uint8_t)Index::A2;
        enum class State
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
        bool _isConnected[MaxIndex + 1];
        State _currentState = State::Undefined;
        unsigned long _leavingTS = 0;
        unsigned long _blinkTS = 0;
        bool _dashboardLit = false;

        LightControl(const LightControl &) = delete;
        LightControl &operator=(const LightControl &) = delete;
    };
} // namespace BusDashboard
