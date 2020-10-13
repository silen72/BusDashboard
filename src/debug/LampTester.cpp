#include "debug/LampTester.h"

namespace BusDashboard {

    LampTester::LampTester(LampHandler& lh) : _lh(&lh) {
        for (uint8_t idx = 0; idx < 64; idx++) {
            _state[idx] = false;
        }
    }

    void LampTester::setCurrentState(const uint8_t button, const bool state) {
        if (_state[button] == state) return;

        Serial.print(F("button "));
        Serial.print((int)button);
        Serial.print(F(" now "));
        if (!state) Serial.print(F("dis"));
        Serial.println(F("connected"));

        _lampposition = 4;

        const bool currentState = _lh->state(_lampposition);

        _lh->setState(_lampposition, !currentState);
        /*_lh->setState(_prevposition, false);
        _prevposition = _lampposition;
        _lampposition++;
        if (_lampposition >= LampHandler::NUMBER_OF_ICS * 8) _lampposition = 0;
        */
        _state[button] = state;
    }

} // namespace BusDashboard class 