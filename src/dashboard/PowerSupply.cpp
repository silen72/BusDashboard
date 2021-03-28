#include <Arduino.h>
#include "dashboard/PowerSupply.h"
#include "dashboard/PowerStateListener.h"

namespace BusDashboard
{
    PowerSupply::PowerSupply(const uint8_t relayPin) : _relayPin(relayPin)
    {
    }

    void PowerSupply::activate()
    {
        _targetState = PowerState::ACTIVE;
    }

    void PowerSupply::addListener(PowerStateListener &listener)
    {
        if (nullptr == _stateListener)
        {
            _stateListener = new ItemNode<PowerStateListener>(listener);
            return;
        }
        _stateListener->append(listener);
    }

    void PowerSupply::begin()
    {
        pinMode(_relayPin, OUTPUT);
        setRelayState(false);
        setState(PowerState::NOT_ACTIVE);
    }

    void PowerSupply::deactivate()
    {
        _targetState = PowerState::NOT_ACTIVE;
    }

    void PowerSupply::notifyListeners()
    {
        ItemNode<PowerStateListener> *node = _stateListener;
        while (nullptr != node)
        {
            node->item()->receiveState(_currentState);
            node = node->next();
        }
    }

    void PowerSupply::setRelayState(const bool state)
    {
        digitalWrite(_relayPin, state ? HIGH : LOW);
    }

    void PowerSupply::setState(const PowerState newState)
    {
        if (newState != _currentState)
        {
            _currentState = newState;
            notifyListeners();
        }
    }

    void PowerSupply::update()
    {
        if (_targetState != _currentState)
        {
            switch (_currentState)
            {
            case PowerState::ACTIVE:
                setState(PowerState::BEFORE_DEACTIVATION);
                break;
            case PowerState::NOT_ACTIVE:
                setState(PowerState::BEFORE_ACTIVATION);
                break;
            case PowerState::BEFORE_ACTIVATION:
                setRelayState(true);
                setState(PowerState::AFTER_ACTIVATION);
                break;
            case PowerState::AFTER_ACTIVATION:
                setState(PowerState::ACTIVE);
                break;
            case PowerState::BEFORE_DEACTIVATION:
                setRelayState(false);
                setState(PowerState::AFTER_DEACTIVATION);
                break;
            case PowerState::AFTER_DEACTIVATION:
                setState(PowerState::NOT_ACTIVE);
                break;
            case PowerState::NOT_INITIALIZED:
                break;
            default:
                break;
            }
        }
    }
}