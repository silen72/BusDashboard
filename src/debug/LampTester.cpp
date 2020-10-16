#include "debug/LampTester.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard {

    void LampTester::registerWith(ButtonHandler &bh)
    {
        // add a test listener to all buttons
        for (uint8_t button = 0; button < LampHandler::NUMBER_OF_ICS * 8; button++)
        {
            bh.addListener(*this, button);
        }
    }

        LampTester::LampTester(LampHandler &lh) : _lh(&lh)
    {
        for (uint8_t idx = 0; idx < 64; idx++) {
            _state[idx] = false;
        }
    }

    bool LampTester::setCurrentState(const uint8_t button, const bool state)
    {
        if (_state[button] == state) return false;

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
        return true;
    }

} // namespace BusDashboard class 