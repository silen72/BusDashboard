#pragma once
#include <Arduino.h>
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class InactivityWarner
    {

    public:

        enum WarningState // three "knock knocks" with the relay
        {
            WARN_START,
            WARN_1_ON,
            WARN_1_OFF,
            WARN_2_ON,
            WARN_2_OFF,
            WARN_3_ON,
            WARN_3_OFF,
            DONE
        };

        /**
        * @param lampPosition the position on the lampdriver for the lamp (or relay) to be activated for a warning
        */
        InactivityWarner(LampHandler::DriverPosition lampPosition);

        /**
         * @return whether a warning is currently happening
         */
        bool warningActive() const { return currentState() != DONE; }
        
        /**
         * @return the current state of warning
         */
        WarningState currentState() const { return _currentState; }

        /**
         * initializes the instance
         */
        void begin();

        /*
        * starts a warning
        */
        void startWarning();

        /*
        * end a warning (interrupts at any point within the warning process)
        */
        void stopWarning();

        /**
         * needs to be called periodically, updates the state the warning is in
         * after calling startWarning() and as long as warningActive() is true
         */
        void update();

    protected:
    private:
        LampHandler::DriverPosition _lampPosition;
        WarningState _currentState = DONE;
        uint32_t _tsLastStateChange = 0;
        uint32_t const WAIT_MS = 200;

        void changeState(bool const relayState, WarningState nextState, bool const force);
        void changeState(bool const relayState, WarningState nextState);

        InactivityWarner() = delete;
        InactivityWarner(const InactivityWarner &) = delete;
        InactivityWarner &operator=(const InactivityWarner &) = delete;
    };
}