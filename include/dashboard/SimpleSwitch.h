#pragma once
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class SimpleSwitch : public ButtonListener
    {
        public:
            SimpleSwitch(char const sendOn,
                         char const sendOff,
                         ButtonHandler::MatrixPosition const btn_position,
                         LampHandler::DriverPosition const lamp_position) : _sendOn(sendOn),
                                                                            _sendOff(sendOff),
                                                                            _btn_position(btn_position),
                                                                            _lmp_position((uint8_t)lamp_position){};
            SimpleSwitch(char const sendOn,
                         char const sendOff,
                         ButtonHandler::MatrixPosition const btn_position) : _sendOn(sendOn),
                                                                             _sendOff(sendOff),
                                                                             _btn_position(btn_position),
                                                                             _lmp_position((uint8_t)0){};
            void setCurrentState(const uint8_t button, const bool state, const bool prev_state);
            void registerWith(ButtonHandler &bh);
            void begin();

        protected:

        private:
            char _sendOn = 0;
            char _sendOff = 0;
            ButtonHandler::MatrixPosition _btn_position;
            uint8_t _lmp_position = 0;
            SimpleSwitch() = delete;
            SimpleSwitch(const SimpleSwitch &) = delete;
            SimpleSwitch &operator=(const SimpleSwitch &) = delete;
    };
}