#pragma once
#include "buttonmatrix/ButtonHandler.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class SimpleSwitch : public ButtonListener
    {
        enum class LightMode
        {
            AlwaysOn,
            AlwaysOff,
            WhenOn,
            WhenOff,
            WhenDashboardIsLit
        };

    public:
        SimpleSwitch(char const sendOn,
                     char const sendOff,
                     ButtonHandler::MatrixPosition const btn_position,
                     LampHandler::DriverPosition const lamp_position,
                     LightMode const lightmode) : _sendOn(sendOn),
                                                  _sendOff(sendOff),
                                                  _btn_position(btn_position),
                                                  _lmp_position(lamp_position),
                                                  _lightmode(lightmode){};

        SimpleSwitch(char const sendOn,
                     char const sendOff,
                     ButtonHandler::MatrixPosition const btn_position,
                     LampHandler::DriverPosition const lamp_position) : SimpleSwitch(sendOn, sendOff, btn_position, lamp_position, LightMode::WhenDashboardIsLit){};

        SimpleSwitch(char const sendOn,
                     char const sendOff,
                     ButtonHandler::MatrixPosition const btn_position,
                     LightMode const lightmode) : SimpleSwitch(sendOn, sendOff, btn_position, LampHandler::DriverPosition::None, lightmode){};

        SimpleSwitch(char const sendOn,
                     char const sendOff,
                     ButtonHandler::MatrixPosition const btn_position) : SimpleSwitch(sendOn, sendOff, btn_position, LampHandler::DriverPosition::None, LightMode::WhenDashboardIsLit){};

        void setCurrentState(const uint8_t button, const bool state, const bool prev_state);
        void registerWith(ButtonHandler &bh);
        void begin() {};

    protected:
    private:
        char _sendOn = 0;
        char _sendOff = 0;
        ButtonHandler::MatrixPosition _btn_position;
        LampHandler::DriverPosition _lmp_position;
        LightMode _lightmode;
        SimpleSwitch() = delete;
        SimpleSwitch(const SimpleSwitch &) = delete;
        SimpleSwitch &operator=(const SimpleSwitch &) = delete;
    };
} // namespace BusDashboard