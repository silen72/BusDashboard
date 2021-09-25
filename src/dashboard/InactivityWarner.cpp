#include "dashboard/InactivityWarner.h"
#include "dashboard/Dashboard.h"

namespace BusDashboard
{
    InactivityWarner::InactivityWarner(LampHandler::DriverPosition lampPosition) : _lampPosition(lampPosition)
    {
    }

    void InactivityWarner::begin()
    {
        //stopWarning();
    }

    void InactivityWarner::startWarning()
    {
        if (_currentState == DONE)
            changeState(false, WARN_START, true);
    }

    void InactivityWarner::stopWarning()
    {
        if (_currentState != DONE)
            changeState(false, DONE, true);
    }

    void InactivityWarner::changeState(bool const relayState, WarningState const nextState, bool const force)
    {
        if (force || (millis() - _tsLastStateChange > WAIT_MS))
        {
            Dashboard::instance().lampHandler().setState(_lampPosition, relayState);
            _tsLastStateChange = millis();
            _currentState = nextState;
        }
    }

    void InactivityWarner::changeState(bool const relayState, WarningState const nextState)
    {
        changeState(relayState, nextState, false);
    }

    void InactivityWarner::update()
    {
        switch (_currentState)
        {
        case WarningState::WARN_START:
            changeState(false, WARN_1_ON, true);
            break;
        case WarningState::WARN_1_ON:
            changeState(true, WARN_1_OFF, true);
            break;
        case WarningState::WARN_1_OFF:
            changeState(false, WARN_2_ON);
            break;
        case WarningState::WARN_2_ON:
            changeState(true, WARN_2_OFF);
            break;
        case WarningState::WARN_2_OFF:
            changeState(false, WARN_3_ON);
            break;
        case WarningState::WARN_3_ON:
            changeState(true, WARN_3_OFF);
            break;
        case WarningState::WARN_3_OFF:
            changeState(false, DONE);
            break;
        default:
            break;
        }
    }
}
