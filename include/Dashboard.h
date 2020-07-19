#pragma once
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
        
    protected:
    
    private:
        static ButtonHandler* _buttonHandler;
        //static KeyboardHandler* _keyboardHandler;
        static KomsiHandler* _komsiHandler;
        //static LampHandler* _lampHandler;

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
    };
}
