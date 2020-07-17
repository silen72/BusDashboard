#include "Komsi/KOMSIHandler.h"
#include <limits>

namespace BusDashboard {

    void KomsiHandler::processIncoming(const unsigned int incomingByte) {
        /*
        * process commands and values coming from KOMSI
        * KOMSI sends a stream like this every frame: A0B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0Y0Z0a0b0c0d0e0f0
        */
        if (isdigit(incomingByte)) {
            if (_command_value < ((INT_MAX - 1) / 10)) _command_value = _command_value * 10 + incomingByte - '0'; // Zahlenwert des aktuellen Kommandos zusammensetzen
        }
        else {
            notifyListeners();                    // Kommando vollständig übertragen, ausführen
            _command = incomingByte;    // geholter Buchstabe ist nächstes Kommando, dessen Wert zurücksetzen
            _command_value = 0;         // Zahlenwert des nächsten Kommandos zurücksetzen
        }
    }


    // notify all interested listeners about the current command and its value
    void KomsiHandler::notifyListeners() {
        uint8_t index = toIndex(_command);
        if (index >= COMMAND_COUNT) return;
        if (_commandListener[index].size() > 0) {
            for (auto cl : _commandListener[index]) {
                cl->receiveCommand(_command, _command_value);
            }
        }
    }


    bool KomsiHandler::addListener(KomsiCommandListener& listener, const uint8_t command) {
        uint8_t index = toIndex(command);
        if (index >= COMMAND_COUNT) return false;
        _commandListener[index].push_back(&listener);
        return true;
    }


    uint8_t KomsiHandler::toIndex(uint8_t command) {
        if (command >= 'A' && command <= 'Z') return command - 'A';
        if (command >= 'a' && command <= 'z') return command - 'a' + 26;
        return KomsiHandler::COMMAND_COUNT + 1;
    }

}