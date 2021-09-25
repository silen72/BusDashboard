#pragma once
#include "ItemNode.h"

namespace BusDashboard
{
    class PowerStateListener;

    /**
     * handles the power supply
    */
    class PowerSupply
    {

    public:
        PowerSupply(const uint8_t relayPin);

        /**
         * this enum contains all states the power supply may be in
         */
        enum PowerState : uint8_t
        {
            NOT_INITIALIZED,
            BEFORE_ACTIVATION,
            AFTER_ACTIVATION,
            ACTIVE,
            BEFORE_DEACTIVATION,
            AFTER_DEACTIVATION,
            NOT_ACTIVE
        };

        /*
        * activates the power supplies (if not already active)
        */
        void activate();

        /**
         * adds a listener
         * @param listener the listener that shall be added
        */
        void addListener(PowerStateListener &listener);

        /*
        * initializes the power supply relay handler
        * the relay is initially turned off
        */
        void begin();

        /**
         * queries the state the power supply is currently in
         * @return the state
        */
        PowerState currentState() const { return _currentState; }

        /*
        * deactivates the power supplies (if not already deactivated)
        */
        void deactivate();

        /*
        * queries the Arduino pin to which the relay is connected
        */
        uint8_t relayPin() const { return _relayPin; }

        /**
         * polls the Serial input queue for KOMSI commands
         * scans the buttons
         * sets lamps on the dashboard accordingly
         */
        void update();

    protected:
    private:
        const uint8_t _relayPin;
        PowerState _currentState = NOT_INITIALIZED;
        PowerState _targetState = NOT_ACTIVE;
        ItemNode<PowerStateListener> *_stateListener = nullptr;

        // notify all listeners about the current state
        void notifyListeners();

        void setState(const PowerState newState);
        void setRelayState(const bool state);

        PowerSupply(const PowerSupply &) = delete;
        PowerSupply &operator=(const PowerSupply &) = delete;
    };
}