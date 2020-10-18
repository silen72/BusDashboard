#include "debug/LampTester.h"
#include "lampdriver/LampHandler.h"
#include "buttonmatrix/ButtonHandler.h"
#include "dashboard/Dashboard.h"

namespace BusDashboard {

    void LampTester::registerWith(ButtonHandler &bh)
    {
        // add a test listener to all buttons
        bh.addListener(*this, ButtonHandler::MatrixPosition::Asr);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Blinker_Links);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Blinker_Rechts);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Blinker_Warn);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Display);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_D);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_N);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_R);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Haltestellenansage);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Haltestellenbremse);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Hupe);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kindersteuerung_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kindersteuerung_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kneeling_1_ab);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kneeling_1_auf);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kneeling_2_ab);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Kneeling_2_auf);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Licht_Fahrer);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Licht_Fern);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Licht_Innen_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Licht_Innen_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Parkbremse);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Reset);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_3);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_4);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_5);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Retarder_6);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheibenwischer_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheibenwischer_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheibenwischer_3);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::SystemInfo_ab);
        bh.addListener(*this, ButtonHandler::MatrixPosition::SystemInfo_auf);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_3);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_Freigabe);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_Sperren_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Tuer_Sperren_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Wischwasser);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Zuendung_1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Zuendung_2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Zuendung_3);
    }

    LampTester::LampTester()
    {
        for (uint8_t idx = 0; idx < 64; idx++) {
            _state[idx] = false;
        }
    }

    void LampTester::setCurrentState(const uint8_t button, const bool state, const bool prev_state)
    {
        if (_state[button] == state) return;

        Serial.print(F("button "));
        Serial.print((int)button);
        Serial.print(F(" now "));
        if (!state) Serial.print(F("dis"));
        Serial.println(F("connected"));

        _lampposition = 4;

        const bool currentState = Dashboard::instance().lampHandler().state(_lampposition);

        Dashboard::instance().lampHandler().setState(_lampposition, !currentState);
        /*_lh->setState(_prevposition, false);
        _prevposition = _lampposition;
        _lampposition++;
        if (_lampposition >= LampHandler::NUMBER_OF_ICS * 8) _lampposition = 0;
        */
        _state[button] = state;
    }

} // namespace BusDashboard class 