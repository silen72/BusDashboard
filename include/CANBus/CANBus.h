#pragma once
#include <Arduino.h>
#include "Dashboard.h"

namespace BusDashboard {

    class Dashboard;

    class CANBus
    {
    public:
        CANBus(Dashboard &parent, const uint8_t pin_cs, const uint8_t pin_nt) : _parent(parent), _pin_cs(pin_cs), _pin_nt(pin_nt) {}
        Dashboard &dashboard() { return _parent; }
        void begin();

    protected:
    private:
        Dashboard &_parent;
        const uint8_t _pin_cs;
        const uint8_t _pin_nt;

        // disallow creation
        CANBus() = delete;
        CANBus(const CANBus &) = delete;
        CANBus &operator=(const CANBus &) = delete;
    };
};