#pragma once
#include <Arduino.h>
#include "Keyboard.h"           // for some strange reasons this include must be the last - otherwise there are lots of strange compile errors ...

namespace BusDashboard {
    /**
     * represents a generic sequence of send-key-actions on the KeyboardHandler action queue
    */
    class KeyAction {
        private:
            /**
            * the KeyAction waiting for this KeyAction to finish (may be nulltr)
            */
            KeyAction* _dependant = nullptr;
            /**
            * the KeyAction this KeyAction is waiting for (may be nulltr)
            */
            KeyAction* _waitFor = nullptr;
            /**
            * the time in milliseconds this KeyAction shall wait after the KeyAction it was waiting for is done
            */
            unsigned long _waitDelay = 0;
            /**
            * the timestamp this has finished its task
            */
            unsigned long _doneTimeStamp = 0;
            /**
            * set the timestamp to indicate that the task is finished
            */
            void setDone();

            // no copy constructor or assign operator
            KeyAction(const KeyAction&) = delete;
            KeyAction& operator=(const KeyAction&) = delete;

        protected:
            /**
            * you need to overwrite this in your derived class
            * this method gets called exactly once
            */
            virtual void doAction() = 0;

        public:
            static const unsigned long DEFAULT_DELAY = 200;
            static const unsigned long DEFAULT_KEYPRESS_DURATION_MS = 250;

            /**
            * call this if you want to create a chain of actions
            * 
            * @param[in] other the KeyAction you want this KeyAction to wait for
            * @param[in] delay the time in milliseconds you want to wait between the other tasks finishing moment and this tasks start
            */
            void waitFor(KeyAction& other, const unsigned long delay);
            /**
            * call this if you want to create a chain of actions with default wait time (DEFAULT_DELAY)
            *
            * @param[in] other the KeyAction you want this KeyAction to wait for
            */
            void waitFor(KeyAction& other);
            /**
            * starts this KeyAction when it is not waiting for another action or other action is done and delay time has passed
            */
            void run();
            /**
            * @returns true when this KeyActions doAction() has been called
            */
            bool done() const { return (_doneTimeStamp > 0); }
            /**
            * @returns the timestamp in milliseconds when this KeyActions doAction() has been called (0 otherwise)
            */
            unsigned long doneTimeStamp() const { return _doneTimeStamp; }
            /**
            * @returns true when this KeyAction and all dependand KeyActions (KeyActions waiting for this) are done
            */
            bool removable() const;
            KeyAction() {}
            virtual ~KeyAction();
    };
}