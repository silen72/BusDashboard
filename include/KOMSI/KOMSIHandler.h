#pragma once
#include <vector>
#include "KomsiCommandListener.h"

namespace BusDashboard {

    class KomsiHandler {
    private:
        static const uint8_t COMMAND_COUNT = 2 * 26;

        std::vector<KomsiCommandListener*> _commandListener[COMMAND_COUNT];
        uint8_t _command = 0;
        int _command_value = 0;

        uint8_t toIndex(uint8_t command);
        void notifyListeners();
        static bool isdigit(const unsigned int value) { return (value >= '0') && (value <= '9'); }

    protected:

    public:

        enum Codes {
            Blinker = 'A',
            C02 = 'B',
            C03 = 'C',
            C04 = 'D',
            C05 = 'E',
            C06 = 'F',
            C07 = 'G',
            C08 = 'H',
            C09 = 'I',
            C10 = 'J',
            C11 = 'K',
            C12 = 'L',
            C13 = 'M',
            C14 = 'N',
            C15 = 'O',
            C16 = 'P',
            C17 = 'Q',
            C18 = 'R',
            C19 = 'S',
            C20 = 'T',
            C21 = 'U',
            C22 = 'V',
            C23 = 'W',
            C24 = 'X',
            C25 = 'Y',
            C26 = 'Z'
        };

        enum Commands {

        };

        void processIncoming(const unsigned int value);
        bool addListener(KomsiCommandListener& listener, const uint8_t command);

    };
}