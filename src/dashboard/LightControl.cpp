#include "dashboard/LightControl.h"

namespace BusDashboard
{
    LightControl::LightControl(LampHandler &lh, KeyboardHandler &kh) : _lh(&lh), _kh(&kh)
    {
        _state[state_A1] = false;
        _state[state_A2] = false;
        _state[state_I1] = false;
        _state[state_I2] = false;
    }

    void LightControl::setCurrentState(const uint8_t button, const bool state)
    {
        bool checkLight = false;
        switch (button)
        {
        case ButtonHandler::MatrixPosition::Scheinwerfer_A1:
            if (_state[state_A1] != state)
            {
                checkLight = true;
                _state[state_A1] = state;
                if (state)
                {
                    switch (_previousState)
                    {
                    case ButtonHandler::MatrixPosition::Scheinwerfer_A2:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                        break;
                    case ButtonHandler::MatrixPosition::Scheinwerfer_I1:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        break;
                    default:
                        break;
                    }
                    _previousState = button;
                }
            }
            else if ((_previousState == button) & !state)
            {
                // _previousState was A1 but A1 was set to off and it (still) should be off => switch is in position off
                _kh->addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                _previousState = 0;
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_A2:
            if (_state[state_A2] != state)
            {
                checkLight = true;
                _state[state_A2] = state;

                if (state)
                {
                    switch (_previousState)
                    {
                    case ButtonHandler::MatrixPosition::Scheinwerfer_A1:
                        _kh->addPressReleaseAction('l');
                        break;
                    case ButtonHandler::MatrixPosition::Scheinwerfer_I2:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        break;
                    default:
                        break;
                    }
                    _previousState = button;
                }
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_I1:
            if (_state[state_I1] != state)
            {
                checkLight = true;
                _state[state_I1] = state;
                if (state)
                {
                    switch (_previousState)
                    {
                    case ButtonHandler::MatrixPosition::Scheinwerfer_I2:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_SHIFT);
                        break;
                    case ButtonHandler::MatrixPosition::Scheinwerfer_A1:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        break;
                    default:
                        break;
                    }
                    _previousState = button;
                }
            }
            break;

        case ButtonHandler::MatrixPosition::Scheinwerfer_I2:
            if (_state[state_I2] != state)
            {
                checkLight = true;
                _state[state_I2] = state;
                if (state)
                {
                    switch (_previousState)
                    {
                    case ButtonHandler::MatrixPosition::Scheinwerfer_I1:
                        _kh->addPressReleaseAction('l');
                        break;
                    case ButtonHandler::MatrixPosition::Scheinwerfer_A2:
                        _kh->addPressReleaseActionMod('l', KEY_LEFT_CTRL);
                        break;
                    default:
                        break;
                    }
                    _previousState = button;
                }
            }
            break;

        default:
            break;
        }

        if (checkLight)
        {
            bool const newState = (_state[state_A1] | _state[state_A2] | _state[state_I1] | _state[state_I2]);
            if (_dashBoardLightsOn != newState)
            {
                _dashBoardLightsOn = newState;
                _lh->setState(LampHandler::DriverPosition::Gangschaltung, newState);
            }
        }
    }

    void LightControl::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_A2);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I1);
        bh.addListener(*this, ButtonHandler::MatrixPosition::Scheinwerfer_I2);
    }

    void LightControl::begin()
    {
    }
}

