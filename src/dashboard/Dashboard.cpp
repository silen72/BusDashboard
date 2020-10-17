#include "dashboard/Dashboard.h"

#include "buttonmatrix/ButtonHandler.h"
#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "CANBus/CANBus.h"

#include "dashboard/LightControl.h"
#include "dashboard/Shifter.h"

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
            // switch off dashboard power relay
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
        // initialise embedded classes
        _buttonHandler = new ButtonHandler(LeonardoPins::BUTTON_MATRIX_CS, ButtonHandler::MCP23S17_ADRS);
        _buttonHandler->begin();
        _lampHandler = new LampHandler(LeonardoPins::LAMP_DRIVER_SI, LeonardoPins::LAMP_DRIVER_SCK, LeonardoPins::LAMP_DRIVER_RCK);
        _lampHandler->begin();
        _keyboardHandler = new KeyboardHandler();
        _keyboardHandler->begin();
        _komsiHandler = new KomsiHandler();
        _komsiHandler->begin();
        _canBusHandler = new CANBus(LeonardoPins::CAN_CS, LeonardoPins::CAN_NT);

        _lightcontrol = new LightControl();
        _lightcontrol->registerWith(*_buttonHandler);
        _lightcontrol->begin();
        _shifter = new Shifter();
        _shifter->registerWith(*_buttonHandler);
        _shifter->begin();

        // initialize own hardware
        pinMode(_pin_relay, OUTPUT);
        digitalWrite(_pin_relay, HIGH);

        // switch off all lamps (a flaw in hw design: the lamps got power with the digitalWrite instruction above, so they probably already have flashed...)
        _lampHandler->allOff();
    }

    Dashboard::Dashboard(const uint8_t pin_relay):_pin_relay(pin_relay) {
        _lastWakeupAction = millis();
    }

    bool Dashboard::isLit()
    {
        return _lightcontrol->isLit();
    }

    void Dashboard::update()
    {
        if (Serial.available())
        {
            int val = Serial.read();
            komsiHandler().processIncoming(val);
            resetIdleTimer();
        }
        buttonHandler().scan();
        lampHandler().update();
        keyboardHandler().update();
        checkIdle();
    }

    Dashboard *Dashboard::_instance = 0;
}
