#include "Dashboard.h"

namespace BusDashboard {
    uint32_t Dashboard::_lastWakeupAction = millis();
    bool Dashboard::_warningGiven = false;
    bool Dashboard::_warningActive = false;
    bool Dashboard::_sleeping = false;

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
            digitalWrite(ArduinoPins::MAINS_RELAY, LOW);
        }
    }

    void Dashboard::resetIdleTimer() {
        _lastWakeupAction = millis();
        if (_sleeping) {
            // back to action: wake up dashboard
            digitalWrite(ArduinoPins::MAINS_RELAY, HIGH);
        }
        _sleeping = false;
        _warningGiven = false;
        _warningActive = false;
    }

    void Dashboard::begin() {
        digitalWrite(ArduinoPins::MAINS_RELAY, HIGH);
    }

}
