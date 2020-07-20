#include "CANBus/CANBus.h"

namespace BusDashboard {
    void CANBus::begin() {
        pinMode(_pin_cs, OUTPUT);
        pinMode(_pin_nt, OUTPUT);
    }
}
