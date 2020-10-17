/*
    the shifter buttons are lit with their own pcb (out of control for this code)
    when at least the sidelight is on
    so the power toggling is done in the lighting control class
*/

#include "dashboard/Shifter.h"
#include "keyboard/KeyboardHandler.h"
#include "dashboard/Dashboard.h"

namespace BusDashboard
{
    Shifter::Shifter(Dashboard &parent) : _parent(parent)
    {
        uint8_t index = (uint8_t)Index::D;
        _state[index] = false;
        _charToSend[index] = 'd';

        index = (uint8_t)Index::R;
        _state[index] = false;
        _charToSend[index] = 'r';

        index = (uint8_t)Index::N;
        _state[index] = false;
        _charToSend[index] = 'n';
    }

    bool Shifter::setCurrentState(const uint8_t button, const bool state)
    {
        uint8_t index;
        switch (button)
        {
        case ButtonHandler::MatrixPosition::Gangschaltung_D:
            index = (uint8_t)Index::D;
            break;
        case ButtonHandler::MatrixPosition::Gangschaltung_R:
            index = (uint8_t)Index::R;
            break;
        case ButtonHandler::MatrixPosition::Gangschaltung_N:
            index = (uint8_t)Index::N;
            break;
        default:
            index = MAX_INDEX+1;
            break;
        }

        bool changed;
        if (index <= MAX_INDEX)
        {
            changed = _state[index] != state;
            if (changed)
            {
                _state[index] = state;
                if (state)
                {
                    _parent.keyboardHandler().addPressReleaseAction(_charToSend[index]);
                }
                else if (!(_state[(uint8_t)Index::D] | _state[(uint8_t)Index::R] | _state[(uint8_t)Index::N]))
                {
                    // it is possible to switch all three buttons off (which should not happen) -> if that's the case set the state to "n"
                    index = (uint8_t)Index::N;
                    _state[index] = true;
                    _parent.keyboardHandler().addPressReleaseAction(_charToSend[index]);
                }
            }
        }
        else
        {
            changed = false;
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