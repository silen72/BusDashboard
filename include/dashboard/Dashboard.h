#pragma once
#include <Arduino.h>

namespace BusDashboard
{

    class ButtonHandler;
    class LampHandler;
    class KeyboardHandler;
    class KomsiHandler;
    class CANBus;
    class PowerSupply;
    class InactivityWarner;

    class LightControl;
    class Shifter;
    class SimpleSwitch;
    class SimplePushButton;
    class ButtonListener;

    // Pins used to connect to the various PCBs
    enum LeonardoPins : uint8_t
    {
        //LAMP_DRIVER_RESET = 3, // not actually connected yet, preparation for board revision 2.0
        LAMP_DRIVER_SI = 4,    // connected to the first 74HC595's SI-pin
        LAMP_DRIVER_RCK = 5,   // connected to all 74HC595's RCLK-pins
        LAMP_DRIVER_SCK = 6,   // connected to all 74HC595's SRCLK-pins
        POWER_RELAY = 7,       // toggle power for the transformers using this pin
        CAN_CS = 8,            // CAN-Bus driver chip select (SPI)
        CAN_NT = 9,            // CAN-Bus driver NT (?) (SPI)
        BUTTON_MATRIX_CS = 10, // button matrix (SPI)
    };

    class Dashboard
    {

    public:
        static Dashboard &instance()
        {
            static CGuard g; // Speicherbereinigung
            if (!_instance)
                _instance = new Dashboard();
            return *_instance;
        }

        enum IdleState : uint8_t
        {
            ACTIVE,
            WARNING,
            WARNED,
            IDLE
        };

        Dashboard(const uint8_t pin_relay);

        /**
         * initialization: turn on mains relay
         */
        void begin();

        /**
         * @returns the buttonhandler instance
         */
        ButtonHandler &buttonHandler() { return *_buttonHandler; };

        /**
         * @returns the canbushandler instance
         */
        CANBus &canBusHandler() { return *_canBusHandler; };

        /**
         * @returns the time in ms that have passed without any waking actions
         */
        uint32_t idleTimeMs() const { return millis() - _lastActionTS; }

        /**
         * @returns wether the dashboard is lit
         */
        bool isLit();

        /**
         * @returns the keyboardhandler instance
         */
        KeyboardHandler &keyboardHandler() { return *_keyboardHandler; };

        /**
         * @returns the komsihandler instance
         */
        KomsiHandler &komsiHandler() { return *_komsiHandler; };

        /**
         * @returns the lamphandler instance
         */
        LampHandler &lampHandler() { return *_lampHandler; };

        /**
         * @returns the powersupplyhandler instance
         */
        PowerSupply &powerSupply() { return *_powerSupply; };

        /**
         * resets the idleTimer
         * activates the power supply
         * stops any pending idle warnings
         */
        void resetIdleTimer();

        /**
         * polls the Serial input queue for KOMSI commands
         * scans the buttons
         * sets lamps on the dashboard accordingly
         */
        void update();

    protected:
    private:
        static Dashboard *_instance;

        class CGuard
        {
        public:
            ~CGuard()
            {
                if (NULL != Dashboard::_instance)
                {
                    delete Dashboard::_instance;
                    Dashboard::_instance = NULL;
                }
            }
        };

        static const uint32_t MAX_IDLE_MS = 15L * 60L * 1000L; // if within this amount of time (in ms) no button has been used the dashboard is considered to be idle
        static const uint32_t WARN_IDLE_MS = 30L * 1000L;      // this amount of time (in ms) before switching off the mains relay, the dashboard gives a warning

        IdleState _idleState = ACTIVE;
        uint32_t _lastActionTS;

        // parts of the dashboard
        ButtonHandler *_buttonHandler;
        LampHandler *_lampHandler;
        KeyboardHandler *_keyboardHandler;
        KomsiHandler *_komsiHandler;
        CANBus *_canBusHandler;
        PowerSupply *_powerSupply;
        InactivityWarner *_inactivityWarner;

        LightControl *_lightcontrol;
        Shifter *_shifter;

        SimplePushButton *_asr;
        SimpleSwitch *_interiorLightingLvl1;
        SimpleSwitch *_interiorLightingLvl2;
        SimpleSwitch *_driverCompartmentLighting;
        SimplePushButton *_announcementOfStops;
        SimplePushButton *_reset;
        SimplePushButton *_parkingBrake;
        SimpleSwitch *_busStopBrake;
        SimplePushButton *_kneeling1up;
        SimplePushButton *_kneeling1down;
        SimplePushButton *_kneeling2up;
        SimplePushButton *_kneeling2down;
        SimpleSwitch *_doorLock1;
        SimpleSwitch *_doorLock2;
        SimpleSwitch *_doorRelease;
        SimplePushButton *_door1;
        SimplePushButton *_door2;
        SimplePushButton *_door3;

        /**
         * controls the mains relay (turns it off after idle time threshold is met)
         * emits a warning
         */
        void checkIdle();

        /*
        * adds a ButtonListener to the ButtonHandler, then initialises the listener
        */
        void initUnit(ButtonListener *bl);

        // disallow creation
        Dashboard();
        Dashboard(const Dashboard &) = delete;
        Dashboard &operator=(const Dashboard &) = delete;
        ~Dashboard() {}
    };
} // namespace BusDashboard
