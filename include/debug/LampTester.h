#pragma once
#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard {
    class LampHandler;
    class LampTester : public ButtonListener {
        public:
            bool setCurrentState(const uint8_t button, const bool state);
            void registerWith(ButtonHandler &bh);
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