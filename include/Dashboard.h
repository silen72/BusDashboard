#pragma once
//#include "buttonmatrix/ButtobHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard {

    /**
     * this is the root class
     */
    class Dashboard {
    public:
        //static ButtonHandler* buttonHandler();
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
        //static ButtonHandler* _buttonHandler;
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
    };
}
