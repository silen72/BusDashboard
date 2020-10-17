#pragma once
#include <Arduino.h>

namespace BusDashboard {

    class ButtonHandler;
    class LampHandler;
    class KeyboardHandler;
    class KomsiHandler;
    class CANBus;

    class LightControl;
    class Shifter;

    // Pins used to connect to the various PCBs
    enum LeonardoPins
    {
        LAMP_DRIVER_SI = 4,    // connected to the first 74HC595's SI-pin
        LAMP_DRIVER_RCK = 5,   // connected to all 74HC595's RCLK-pins
        LAMP_DRIVER_SCK = 6,   // connected to all 74HC595's SRCLK-pins
        POWER_RELAY = 7,       // toggle power for the transformers using this pin
        CAN_CS = 8,            // CAN-Bus driver chip select (SPI)
        CAN_NT = 9,            // CAN-Bus driver NT (?) (SPI)
        BUTTON_MATRIX_CS = 10, // button matrix (SPI)
    };

    class Dashboard {

        public:
            
            static Dashboard &instance()
            {
                static CGuard g; // Speicherbereinigung
                if (!_instance)
                    _instance = new Dashboard(LeonardoPins::POWER_RELAY);
                return *_instance;
            }
            void xyz(){};

            static const uint16_t WARN_BLINK_DELAY_MS = 200; // when a button signals a problem, this is the delay time in ms between toggling the lamp

            /**
         * polls the Serial input queue for KOMSI commands
         * scans the buttons
         * sets lamps on the dashboard accordingly
         */
            void update();

            /**
         * resets the idleTimer
         */
            void resetIdleTimer();

            /**
         * initialization: turn on mains relay
         */
            void begin();

            /**
         * @returns the time in ms that have past without any waking actions
         */
            uint32_t sleepTimeMs() const { return millis() - _lastWakeupAction; }

            bool isLit();

            Dashboard(const uint8_t pin_relay);

            // hardware handler classes
            ButtonHandler &buttonHandler() { return *_buttonHandler; };
            LampHandler &lampHandler() { return *_lampHandler; };
            KeyboardHandler &keyboardHandler() { return *_keyboardHandler; };
            KomsiHandler &komsiHandler() { return *_komsiHandler; };
            CANBus &canBusHandler() { return *_canBusHandler; };

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

            static const uint32_t MAX_IDLE_MS = 15L * 60L * 1000L; // if within this amount of time (in ms) no button has been used an no command has been received, the dashboard is considered to be idle
            static const uint32_t WARN_IDLE_MS = 30L * 1000L;      // this amount of time (in ms) before switching off the mains relay, the dashboard gives a warning

            const uint8_t _pin_relay;
            uint32_t _lastWakeupAction;
            bool _warningGiven = false;
            bool _warningActive = false;
            bool _sleeping = false;

            // parts of the dashboard
            ButtonHandler *_buttonHandler;
            LampHandler *_lampHandler;
            KeyboardHandler *_keyboardHandler;
            KomsiHandler *_komsiHandler;
            CANBus *_canBusHandler;

            LightControl *_lightcontrol;
            Shifter *_shifter;

            /**
         * controls the mains relay (turns it off after idle time threshold is met)
         * emits a warning
         */
            void checkIdle();

            // disallow creation
            Dashboard() = delete;
            Dashboard(const Dashboard &) = delete;
            Dashboard &operator=(const Dashboard &) = delete;
            ~Dashboard() {}
    };
}
