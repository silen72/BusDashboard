#pragma once
#include "Arduino.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard {
    class LampTester : public ButtonListener {
        public:
            void setCurrentState(const uint8_t button, const bool state);
            LampTester(LampHandler& lh);

        protected:

        private:
            bool _state[64];
            LampHandler* _lh;
            uint8_t _lampposition = 0;
            uint8_t _prevposition = 1;

            LampTester() = delete;
            LampTester(const LampTester &) = delete;
            LampTester &operator=(const LampTester &) = delete;
    };
}