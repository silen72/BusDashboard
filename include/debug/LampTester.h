#pragma once
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard {
    class LampTester : public ButtonListener {
        public:
            void setCurrentState(const uint8_t button, const bool state, bool prev_state);
            void registerWith(ButtonHandler &bh);
            LampTester();
            void begin() {}

        protected:

        private:
            bool _state[64];
            uint8_t _lampposition = 0;
            uint8_t _prevposition = 1;

            LampTester(const LampTester &) = delete;
            LampTester &operator=(const LampTester &) = delete;
    };
}