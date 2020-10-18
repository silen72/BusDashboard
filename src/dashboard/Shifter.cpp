/*
    the shifter buttons are lit with their own pcb (out of control for this code)
    when at least the sidelight is on
    so the power toggling is done in the lighting control class
*/

#include "dashboard/Shifter.h"
#include "keyboard/KeyboardHandler.h"
#include "dashboard/Dashboard.h"
#include "lampdriver/LampHandler.h"
#include "buttonmatrix/ButtonHandler.h"

namespace BusDashboard
{
    Shifter::Shifter()
    {
    }

    void Shifter::setCurrentState(const uint8_t button, const bool state, const bool prev_state)
    {
        State targetState;

        if (state != prev_state)
        {
            switch (button)
            {
            case (uint8_t)ButtonHandler::MatrixPosition::Gangschaltung_D:
                if (state)
                    targetState = State::D;
                else
                    targetState = State::N;
                break;
            case (uint8_t)ButtonHandler::MatrixPosition::Gangschaltung_R:
                if (state)
                    targetState = State::R;
                else
                    targetState = State::N;
                break;
            case (uint8_t)ButtonHandler::MatrixPosition::Gangschaltung_N:
                if (state)
                    targetState = State::N;
                break;
            default:
                break;
            }
        }

        if (targetState != _state)
        {
            char charToSend;
            switch (targetState)
            {
            case State::D:
                charToSend = 'd';
                break;
            case State::N:
                charToSend = 'n';
                break;
            case State::R:
                charToSend = 'r';
                break;
            case State::Undefined:
                charToSend = 0;
                break;
            }
            if (charToSend > 0)
                Dashboard::instance().keyboardHandler().addPressReleaseAction(charToSend);
            targetState = _state;
        }
        Dashboard::instance().lampHandler().setState(LampHandler::DriverPosition::Gangschaltung, Dashboard::instance().isLit());
    }

    void Shifter::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_D);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_R);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_N);
    }
} // namespace BusDashboard