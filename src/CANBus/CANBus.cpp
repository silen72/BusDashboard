#include "CANBus/CANBus.h"

//#include "Dashboard.h"

namespace BusDashboard {
    void CANBus::begin() {
        pinMode(_pin_cs, OUTPUT);
        pinMode(_pin_nt, OUTPUT);
    }
}
