#include <Keyboard.h>
#include "dashboard/Dashboard.h"

#include "buttonmatrix/ButtonHandler.h"
#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "CANBus/CANBus.h"

#include "dashboard/LightControl.h"
#include "dashboard/PowerSupply.h"
#include "dashboard/Shifter.h"
#include "dashboard/SimplePushButton.h"
#include "dashboard/SimpleSwitch.h"


namespace BusDashboard
{

    Dashboard::Dashboard()
    {
        _lastWakeupAction = millis();
    }

    void Dashboard::checkIdle()
    {
        uint32_t idleTime = millis() - _lastWakeupAction;
        if (idleTime < Dashboard::WARN_IDLE_MS)
            return;

        if (idleTime < Dashboard::MAX_IDLE_MS)
        {
            if (!_warningGiven)
            {
                if (_warningActive)
                {
                    // ToDo: emit a warning (without delays)
                    _warningGiven = true;
                }
                else
                {
                    _warningActive = true;
                }
            }
        }
        else
        {
            powerSupply().deactivate(); // switch off dashboard power relay
        }
    }

    void Dashboard::resetIdleTimer()
    {
        _lastWakeupAction = millis();
        powerSupply().activate();
        _sleeping = false;
        _warningGiven = false;
        _warningActive = false;
    }

    void Dashboard::initUnit(ButtonListener *bl)
    {
        bl->registerWith(*_buttonHandler);
        bl->begin();
    }

    void Dashboard::begin()
    {
        // initialize own hardware
        resetIdleTimer();

        // switch off all lamps (a flaw in hw design: the lamps got power with the digitalWrite instruction above, so they probably already have flashed...)
        _lampHandler = new LampHandler(LeonardoPins::LAMP_DRIVER_SI, LeonardoPins::LAMP_DRIVER_SCK, LeonardoPins::LAMP_DRIVER_RCK);
        _lampHandler->begin();
        _lampHandler->allOff();

        // initialise embedded classes
        _buttonHandler = new ButtonHandler(LeonardoPins::BUTTON_MATRIX_CS, ButtonHandler::MCP23S17_ADRS);
        _buttonHandler->begin();
        _keyboardHandler = new KeyboardHandler();
        _keyboardHandler->begin();
        _komsiHandler = new KomsiHandler();
        _komsiHandler->begin();
        _canBusHandler = new CANBus(LeonardoPins::CAN_CS, LeonardoPins::CAN_NT);
        _canBusHandler->begin();
        _powerSupply = new PowerSupply(LeonardoPins::POWER_RELAY);
        _powerSupply->begin();

        // initialise knobs, handles, buttons, switches
        _lightcontrol = new LightControl();
        initUnit(_lightcontrol);
        _asr = new SimplePushButton('a', ButtonHandler::MatrixPosition::Asr, LampHandler::DriverPosition::ASR);
        initUnit(_asr);
        _interiorLightingLvl1 = new SimpleSwitch('8', '8', ButtonHandler::MatrixPosition::Licht_Innen_1, LampHandler::DriverPosition::Licht_Innen);
        initUnit(_interiorLightingLvl1);
        _interiorLightingLvl2 = new SimpleSwitch('9', '8', ButtonHandler::MatrixPosition::Licht_Innen_2);
        initUnit(_interiorLightingLvl2);
        _driverCompartmentLighting = new SimpleSwitch('6', '6', ButtonHandler::MatrixPosition::Licht_Fahrer, LampHandler::DriverPosition::Licht_Fahrer);
        initUnit(_driverCompartmentLighting);
        // Warnblinkanlage
        _announcementOfStops = new SimplePushButton('q', ButtonHandler::MatrixPosition::Haltestellenansage, LampHandler::DriverPosition::Haltestellenansage);
        initUnit(_announcementOfStops);
        // Multihebel
        // Retarderhebel
        // Zündung
        // Display
        // Systeminfo
        _parkingBrake = new SimplePushButton('.', ButtonHandler::MatrixPosition::Parkbremse, LampHandler::DriverPosition::Parkbremse);
        initUnit(_parkingBrake);
        _busStopBrake = new SimpleSwitch((uint8_t)KeyboardHandler::Keys::KEY_SCROLL_LOCK, (uint8_t)KeyboardHandler::Keys::KEY_SCROLL_LOCK, ButtonHandler::MatrixPosition::Parkbremse, LampHandler::DriverPosition::Parkbremse);
        initUnit(_busStopBrake);
        // Kindersteuerung links
        // Kindersteuerung rechts
        _kneeling1up = new SimplePushButton(KEY_UP_ARROW, ButtonHandler::MatrixPosition::Kneeling_1_auf, LampHandler::DriverPosition::Kneeling_1);
        initUnit(_kneeling1up);
        _kneeling1down = new SimplePushButton(KEY_DOWN_ARROW, ButtonHandler::MatrixPosition::Kneeling_1_ab);
        initUnit(_kneeling1down);
        _kneeling2up = new SimplePushButton(KEY_UP_ARROW, ButtonHandler::MatrixPosition::Kneeling_2_auf, LampHandler::DriverPosition::Kneeling_2);
        initUnit(_kneeling2up);
        _kneeling2down = new SimplePushButton(KEY_DOWN_ARROW, ButtonHandler::MatrixPosition::Kneeling_2_ab);
        initUnit(_kneeling2down);

        //_doorLock1 = new SimpleSwitch('6', '6', ButtonHandler::MatrixPosition::Licht_Fahrer, LampHandler::DriverPosition::Licht_Fahrer);
        //_doorLock2 = new SimpleSwitch('6', '6', ButtonHandler::MatrixPosition::Licht_Fahrer, LampHandler::DriverPosition::Licht_Fahrer);
        _doorRelease = new SimpleSwitch((uint8_t)KeyboardHandler::Keys::KEYPAD_MINUS, (uint8_t)KeyboardHandler::Keys::KEYPAD_MINUS, ButtonHandler::MatrixPosition::Tuer_Freigabe, LampHandler::DriverPosition::Tuer_Freigabe);
        initUnit(_doorRelease);
        _door1 = new SimplePushButton((uint8_t)KeyboardHandler::Keys::KEYPAD_SLASH, ButtonHandler::MatrixPosition::Tuer_1, LampHandler::DriverPosition::Tuer_1);
        _door2 = new SimplePushButton((uint8_t)KeyboardHandler::Keys::KEYPAD_ASTERISK, ButtonHandler::MatrixPosition::Tuer_2, LampHandler::DriverPosition::Tuer_2);
        _door3 = new SimplePushButton(KEY_F7, ButtonHandler::MatrixPosition::Kneeling_1_auf, LampHandler::DriverPosition::Tuer_3);

        _shifter = new Shifter();
        initUnit(_shifter);

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
        powerSupply().update();
    }

    Dashboard *Dashboard::_instance = 0;
} // namespace BusDashboard
