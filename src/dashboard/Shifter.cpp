/*
    the shifter buttons are lit with their own pcb (out of control for this code)
    when at least the sidelight is on
    so the power toggling is done in the lighting control class
*/

#include "dashboard/Shifter.h"

namespace BusDashboard
{
    Shifter::Shifter(KeyboardHandler &kh) : _kh(&kh)
    {
        _state[state_D] = false;
        _state[state_R] = false;
        _state[state_N] = false;
    }

    void Shifter::setCurrentState(const uint8_t button, const bool state)
    {
        bool checkNone = false;
        switch (button)
        {
        case ButtonHandler::MatrixPosition::Gangschaltung_D:
            if (_state[state_D] != state)
            {
                if (state)
                {
                    _kh->addPressReleaseAction('d');
                }
                else
                {
                    checkNone = true;
                }
                _state[state_D] = state;
            }
            break;

        case ButtonHandler::MatrixPosition::Gangschaltung_R:
            if (_state[state_R] != state)
            {
                if (state)
                {
                    _kh->addPressReleaseAction('r');
                }
                else
                {
                    checkNone = true;
                }
                _state[state_R] = state;
            }
            break;

        case ButtonHandler::MatrixPosition::Gangschaltung_N:
            if (_state[state_N] != state)
            {
                if (state)
                {
                    _kh->addPressReleaseAction('n');
                }
                _state[state_N] = state;
            }
            break;

        default:
            break;
        }

        // it is possible to switch all three buttons off (which should not happen) -> if that's the case set the state to "n"
        if (checkNone & (!(_state[state_N] | _state[state_R] | _state[state_D])))
            {
                _state[state_N] = true;
                _kh->addPressReleaseAction('n');
            }
    }

    void Shifter::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_D);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_R);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Gangschaltung_N);
    }

    void Shifter::begin() {
    }
}