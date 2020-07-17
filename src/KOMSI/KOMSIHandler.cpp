#include "Komsi/KOMSIHandler.h"
#include <limits>

namespace BusDashboard {

    void KomsiHandler::processIncoming(const uint16_t incomingByte) {
        if (isdigit(incomingByte)) {
            // avoid overflow
            if (_command_value < ((INT_MAX - 1) / 10)) _command_value = _command_value * 10 + incomingByte - '0'; // calculate command value
        }
        else {
            notifyListeners();          // command completely received, inform listeners
            _command = incomingByte;    // received byte is the next command
            _command_value = 0;         // init command value
        }
    }


    void KomsiHandler::notifyListeners() {
        uint8_t index = toIndex(_command);
        if ( isIndexValid(index) && (_commandListener[index].size() > 0 )) {
            for (auto cl : _commandListener[index]) {
                cl->receiveCommand(_command, _command_value);
            }
        }
    }

    bool KomsiHandler::addListener(KomsiCommandListener& listener, const uint8_t command) {
        uint8_t index = toIndex(command);
        bool valid = isIndexValid(index);
        if (valid) _commandListener[index].push_back(&listener);
        return valid;
    }

    uint8_t KomsiHandler::toIndex(uint8_t command) {
        if (command >= 'A' && command <= 'Z') return command - 'A';
        if (command >= 'a' && command <= 'z') return command - 'a' + 26;
        return KomsiHandler::COMMAND_COUNT + 1; // unknown command
    }

}