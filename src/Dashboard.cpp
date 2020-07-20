#include "Dashboard.h"

namespace BusDashboard {

    void Dashboard::checkIdle() {
        uint32_t idleTime = millis() - _lastWakeupAction;
        if ( idleTime < Dashboard::WARN_IDLE_MS ) return;

        if ( idleTime < Dashboard::MAX_IDLE_MS ) {
            if (!_warningGiven) {
                if (_warningActive) {
                    // ToDo: emit a warning (without delays)
                    _warningGiven = true;
                } else {
                    _warningActive = true;
                }
            }
        } else {
            // switch off dashboard
            digitalWrite(_pin_relay, LOW);
        }
    }

    void Dashboard::resetIdleTimer() {
        _lastWakeupAction = millis();
        if (_sleeping) {
            // back to action: wake up dashboard
            digitalWrite(_pin_relay, HIGH);
        }
        _sleeping = false;
        _warningGiven = false;
        _warningActive = false;
    }

    void Dashboard::begin() {
        pinMode(_pin_relay, OUTPUT);
        digitalWrite(_pin_relay, HIGH);
    }

    Dashboard::Dashboard(const uint8_t pin_relay):_pin_relay(pin_relay) {
        _lastWakeupAction = millis();
    }
}
