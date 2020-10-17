#include "dashboard/LightControl.h"

#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "Keyboard.h"
#include "dashboard/Dashboard.h"
#include "buttonmatrix/ButtonHandler.h"

namespace BusDashboard
{
    LightControl::LightControl()
    {
        _isConnected[(uint8_t)Index::A1] = false;
        _isConnected[(uint8_t)Index::A2] = false;
        _isConnected[(uint8_t)Index::I1] = false;
        _isConnected[(uint8_t)Index::I2] = false;
    }

    bool LightControl::setCurrentState(const uint8_t button, const bool state)
    {
        bool resetTimer = false;
        uint8_t index;
        switch (button)
        {
        case ButtonHandler::MatrixPosition::Scheinwerfer_A1:
            index = (uint8_t)Index::A1;
            if (_isConnected[index] != state)
            {
                _isConnected[index] = state;
                resetTimer = true;
                if (state)
                {
                    switch (_currentState)
                    {
                    case State::Off:
                    case State::A2:
                    case State::A2_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                        _currentState = State::A1;
                        break;
                    case State::I1:
                    case State::I1_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        _currentState = State::A1;
                        break;
                    case State::Undefined:
                        _currentState = State::Wait_for_off;
                        break;
                    default:
                        break;
                    }
                    _leavingTS = 0;
                }
                else
                {
                    _currentState = State::A1_leaving;
                    _leavingTS = millis();
                }
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_A2:
            index = (uint8_t)Index::A2;
            if (_isConnected[index] != state)
            {
                _isConnected[index] = state;
                resetTimer = true;
                if (state)
                {
                    switch (_currentState)
                    {
                    case State::A1:
                    case State::A1_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseAction('l');
                        _currentState = State::A2;
                        break;
                    case State::I2:
                    case State::I2_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        _currentState = State::A2;
                        break;
                    case State::Undefined:
                        _currentState = State::Wait_for_off;
                    default:
                        break;
                    }
                    _leavingTS = 0;
                }
                else
                {
                    _currentState = State::A2_leaving;
                    _leavingTS = millis();
                }
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_I1:
            index = (uint8_t)Index::I1;
            if (_isConnected[index] != state)
            {
                _isConnected[index] = state;
                resetTimer = true;
                if (state)
                {
                    switch (_currentState)
                    {
                    case State::A1:
                    case State::A1_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        _currentState = State::I1;
                        break;
                    case State::I2:
                    case State::I2_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                        _currentState = State::I1;
                        break;
                    case State::Undefined:
                        _currentState = State::Wait_for_off;
                    default:
                        break;
                    }
                    _leavingTS = 0;
                }
                else
                {
                    _currentState = State::I1_leaving;
                    _leavingTS = millis();
                }
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_I2:
            index = (uint8_t)Index::I2;
            if (_isConnected[index] != state)
            {
                _isConnected[index] = state;
                resetTimer = true;
                if (state)
                {
                    switch (_currentState)
                    {
                    case State::A2:
                    case State::A2_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        _currentState = State::I2;
                        break;
                    case State::I1:
                    case State::I1_leaving:
                        Dashboard::instance().keyboardHandler().addPressReleaseAction('l');
                        _currentState = State::I2;
                        break;
                    case State::Undefined:
                        _currentState = State::Wait_for_off;
                    default:
                        break;
                    }
                    _leavingTS = 0;
                }
                else
                {
                    _currentState = State::I2_leaving;
                    _leavingTS = millis();
                }
            }
            break;

        default:
            break;
        }

        // check leaving state duration to detect "off" state of lighting control knob
        if ((_leavingTS > 0) & (millis() - _leavingTS > WAIT_MAX_MS_IN_LEAVING_STATE_MS))
        {
            switch (_currentState)
            {
            case State::I2_leaving:
            case State::A2_leaving:
                Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                // intentional fall through
            case State::I1_leaving:
            case State::A1_leaving:
                Dashboard::instance().keyboardHandler().addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                break;
            default:
                break;
            }
            _currentState = State::Off;
            _leavingTS = 0;
        }

        switch (_currentState)
        {
        case State::Wait_for_off:
            // blink to indicate problem

            // intentional fall through
        case State::Off:
        case State::Undefined:
            _dashboardLit = false;
            break;
        default:
            _dashboardLit = true;
            break;
        }
        
        return resetTimer;
    }

    void LightControl::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I2);
    }

}

