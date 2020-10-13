#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonHandler.h"
#include "CANBus/CANBus.h"
#include "keyboard/KeyboardHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard {


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
        const uint32_t MAX_IDLE_MS = 15L * 60L * 1000L; // if within this amount of time (in ms) no button has been used an no command has been received, the dashboard is considered to be idle
        const uint32_t WARN_IDLE_MS = 30L * 1000L; // this amount of time (in ms) before switching off the mains relay, the dashboard gives a warning
 
        /**
         * controls the mains relay (turns it off after idle time threshold is met)
         * emits a warning
         */
        void checkIdle();

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

        
        Dashboard(const uint8_t pin_relay);

    protected:

    private:
        const uint8_t _pin_relay;
        uint32_t _lastWakeupAction;
        bool _warningGiven = false;
        bool _warningActive = false;
        bool _sleeping = false;

        // disallow creation
        Dashboard() = delete;
        Dashboard(const Dashboard &) = delete;
        Dashboard &operator=(const Dashboard &) = delete;
    };
}
