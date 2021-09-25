#pragma once
#include "buttonmatrix/ButtonHandler.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class SimpleSwitch : public ButtonListener
    {

    public:
        enum class LightMode : uint8_t
        {
            AlwaysOn,
            AlwaysOff,
            WhenOn,
            WhenOff,
            WhenDashboardIsLit,
            Ignore
        };
        SimpleSwitch(uint8_t const sendOn,
                     uint8_t const sendOff,
                     ButtonHandler::MatrixPosition const btn_position,
                     LampHandler::DriverPosition const lamp_position = LampHandler::DriverPosition::None,
                     LightMode const lightmode = LightMode::WhenDashboardIsLit) : _sendOn(sendOn),
                                                                                  _sendOff(sendOff),
                                                                                  _btn_position(btn_position),
                                                                                  _lmp_position(lamp_position),
                                                                                  _lightmode(lightmode){};

        void setCurrentState(const uint8_t button, const bool state, const bool prev_state) override;
        void registerWith(ButtonHandler &bh) override;
        void begin() override {};

    protected:
    private:
        uint8_t _sendOn;
        uint8_t _sendOff;
        ButtonHandler::MatrixPosition _btn_position;
        LampHandler::DriverPosition _lmp_position;
        LightMode _lightmode;
        SimpleSwitch() = delete;
        SimpleSwitch(const SimpleSwitch &) = delete;
        SimpleSwitch &operator=(const SimpleSwitch &) = delete;
    };
} // namespace BusDashboard