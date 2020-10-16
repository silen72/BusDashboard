/*
    the shifter buttons are lit with their own pcb (out of control for this code)
    when at least the sidelight is on
    so the power toggling is done in the lighting control class
*/

#include "dashboard/Shifter.h"
#include "keyboard/KeyboardHandler.h"

namespace BusDashboard
{
    Shifter::Shifter(KeyboardHandler &kh) : _kh(&kh)
    {
        _state[state_D] = false;
        _state[state_R] = false;
        _state[state_N] = false;
    }

    bool Shifter::setCurrentState(const uint8_t button, const bool state)
    {
        bool changed = false;
        bool checkNone = false;
        switch (button)
        {
        case ButtonHandler::MatrixPosition::Gangschaltung_D:
            changed = _state[state_D] != state;
            if (changed)
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
            changed = _state[state_R] != state;
            if (changed)
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
            changed = _state[state_N] != state;
            if (changed)
            {
                if (state)
                {
                    _kh->addPressReleaseAction('n');
                }
                else
                {
                    checkNone = true;
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
        return changed;
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