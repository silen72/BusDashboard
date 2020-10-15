#pragma once
#include "Arduino.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"

namespace BusDashboard {
   
    class Shifter : public ButtonListener {
        public:
            void setCurrentState(const uint8_t button, const bool state);
            void registerWith(ButtonHandler &bh);
            Shifter(LampHandler &lh, KeyboardHandler &kh);
            void begin();

        protected:
        private:
            static const uint8_t state_D = 0;
            static const uint8_t state_R = 1;
            static const uint8_t state_N = 2;
            bool _state[3];
            bool _initDone = false;
            LampHandler *_lh;
            KeyboardHandler *_kh;

            Shifter() = delete;
            Shifter(const Shifter &) = delete;
            Shifter &operator=(const Shifter &) = delete;
    };
} // namespace BusDashboard
