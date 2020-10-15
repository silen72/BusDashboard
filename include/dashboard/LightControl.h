#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"

namespace BusDashboard
{

    class LightControl : public ButtonListener
    {
    public:
        void setCurrentState(const uint8_t button, const bool state);
        void registerWith(ButtonHandler &bh);
        LightControl(LampHandler &lh, KeyboardHandler &kh);
        void begin();

    protected:
    private:
        static const uint8_t state_A1 = 0;
        static const uint8_t state_A2 = 1;
        static const uint8_t state_I1 = 2;
        static const uint8_t state_I2 = 3;
        bool _state[4];
        bool _initDone = false;
        bool _dashBoardLightsOn = false;
        uint8_t _previousState = 0;
        LampHandler *_lh;
        KeyboardHandler *_kh;

        LightControl() = delete;
        LightControl(const LightControl &) = delete;
        LightControl &operator=(const LightControl &) = delete;
    };
} // namespace BusDashboard
