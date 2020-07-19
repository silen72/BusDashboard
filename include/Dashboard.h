#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard {


    // Pins used to connect the various boards
    enum ArduinoPins {
        LAMP_DRIVER_SI = 4,
        LAMP_DRIVER_RCK = 5,
        LAMP_DRIVER_SCK = 6,
        MAINS_RELAY = 7,
        CAN_CS = 8,             // CAN-Bus driver chip select (SPI)
        CAN_NT = 9,             // CAN-Bus driver NT (?) (SPI)
        BUTTON_MATRIX_CS = 10,  // button matrix (SPI)
        SPI_MOSI = 11,          // on a leonardo board this is only available on the ISCP header (ISCP-4)!
        SPI_MISO = 12,          // on a leonardo board this is only available on the ISCP header (ISCP-1)!
        SPI_SCK = 13            // on a leonardo board this is only available on the ISCP header (ISCP-3)!
    };

    

    class Dashboard {
    public:
       static const uint32_t MAX_IDLE_MS = 5L * 60L * 1000L; // if within this amount of time (in ms) no button has been used an no command has been received, the dashboard is considered to be idle
        static const uint32_t WARN_IDLE_MS = 30L * 1000L; // this amount of time (in ms) before switching off the mains relay, the dashboard gives a warning
 
        /**
         * controls the mains relay (turns it off after idle time threshold is met)
         * emits a warning
         */
        static void checkIdle();

        /**
         * resets the idleTimer
         */
        static void resetIdleTimer();

        /**
         * initialization: turn on mains releay
         */
        static void begin();

        /**
         * @returns the time in ms that have past without any waking actions
         */
        static uint32_t sleepTimeMs() { return millis() - _lastWakeupAction; }

    protected:
    
    private:
        static uint32_t _lastWakeupAction;
        static bool _warningGiven;
        static bool _warningActive;
        static bool _sleeping;
    };
}
