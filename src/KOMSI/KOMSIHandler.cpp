#include "Komsi/KOMSIHandler.h"

namespace BusDashboard {

    void KomsiHandler::processIncoming(const int incomingByte) {
        if (isdigit(incomingByte)) {
            // avoid overflow
            if (_command_value < ((UINT16_MAX - 1) / 10)) _command_value = _command_value * 10 + incomingByte - '0'; // calculate command value
        }
        else {
            notifyListeners();          // command completely received, inform listeners
            _command = incomingByte;    // received byte is the next command
            _command_value = 0;         // init command value
        }
    }


    void KomsiHandler::notifyListeners() {
        uint8_t index = toIndex(_command);
        if (!isIndexValid(index)) return;
        ItemNode<KomsiCommandListener> *node = _commandListener[index];
        while (nullptr != node) {
            node->item()->receiveCommand(_command, _command_value);
            node = node->next();
        }
    }

    bool KomsiHandler::addListener(KomsiCommandListener& listener, const uint8_t command) {
        uint8_t index = toIndex(command);
        bool valid = isIndexValid(index);
        if (valid) {
            if (nullptr == _commandListener[index]) {
                _commandListener[index] = new ItemNode<KomsiCommandListener>(listener);
            } else {
                _commandListener[index]->append(listener);
            }
        }
        return valid;
    }

    uint8_t KomsiHandler::toIndex(uint8_t command) {
        if (command >= 'A' && command <= 'Z') return command - 'A';
        if (command >= 'a' && command <= 'z') return command - 'a' + 26;
        return KomsiHandler::COMMAND_COUNT + 1; // unknown command
    }

}