#pragma once
#include <Arduino.h>
namespace BusDashboard {
    class CANBus {
        public:
            CANBus(const uint8_t pin_cs, const uint8_t pin_nt) : _pin_cs(pin_cs), _pin_nt(pin_nt) {}
            void begin();

        protected :

        private:
            const uint8_t _pin_cs;
            const uint8_t _pin_nt;

            // disallow creation
            CANBus() = delete;
            CANBus(const CANBus&) = delete;
            CANBus &operator=(const CANBus&) = delete;
    };
};