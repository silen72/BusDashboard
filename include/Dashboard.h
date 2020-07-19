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
        CAN_CS = 8,             // CAN-Bus driver (SPI)
        CAN_NT = 9,             // CAN-Bus driver (SPI)
        BUTTON_MATRIX_CS = 10,  // button matrix (SPI)
        SPI_MOSI = 11,          // on a leonardo board this is only available on the ISCP header (ISCP-4)!
        SPI_MISO = 12,          // on a leonardo board this is only available on the ISCP header (ISCP-1)!
        SPI_SCK = 13            // on a leonardo board this is only available on the ISCP header (ISCP-3)!
    };

    

    /**
     * this is the root class
     */
    class Dashboard {
    public:

        static const uint32_t MAX_IDLE_MS = 5L * 60L * 1000L; // if within this amount of time (in ms) no button has been used an no command has been received, the dashboard is considered to be idle
        static const uint32_t WARN_IDLE_MS = 30L * 1000L; // this amount of time (in ms) before switching off the mains relay, the dashboard gives a warning
        
        /**
         * @returns the singleton ButtonHandlerHandler
         */
        static ButtonHandler* buttonHandler() {
            static CGuardButtonHandler g;
            if (!_buttonHandler) _buttonHandler = new ButtonHandler(ArduinoPins::BUTTON_MATRIX_CS, ButtonHandler::MCP23S17_ADRS);
			return _buttonHandler;
        }

        /**
         * @returns the singleton KeyboardHandler
         */
        static KeyboardHandler* keyboardHandler() { return KeyboardHandler::instance(); }
        
        /**
         * @returns the singleton KomsiHandler
         */
        static KomsiHandler* komsiHandler() {
            static CGuardKomsiHandler g;
            if (!_komsiHandler) _komsiHandler = new KomsiHandler();
			return _komsiHandler;
        }
        
        /**
         * @returns the singleton LampHandler
         */
        static LampHandler* lampHandler() { return LampHandler::instance(); }

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

    protected:
    
    private:
        static ButtonHandler* _buttonHandler;
        //static KeyboardHandler* _keyboardHandler;
        static KomsiHandler* _komsiHandler;
        //static LampHandler* _lampHandler;
        static uint32_t _lastWakeupAction;
        static bool _warningGiven;
        static bool _warningActive;
        static bool _sleeping;

        class CGuardKomsiHandler {
		public:
			~CGuardKomsiHandler() {
				if (nullptr != Dashboard::_komsiHandler) {
					delete Dashboard::_komsiHandler;
					Dashboard::_komsiHandler = nullptr;
				}
			}
		};

        class CGuardButtonHandler {
		public:
			~CGuardButtonHandler() {
				if (nullptr != Dashboard::_buttonHandler) {
					delete Dashboard::_buttonHandler;
					Dashboard::_buttonHandler = nullptr;
				}
			}
		};

        static uint32_t sleepTimeMs() { return millis() - _lastWakeupAction; }

    };
}
