#pragma once
#include "buttonmatrix/ButtonHandler.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class SimplePushButton : public ButtonListener
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
        SimplePushButton(char const toggleChar,
                         ButtonHandler::MatrixPosition const btn_position,
                         LampHandler::DriverPosition const lamp_position,
                         LightMode const lightmode) : _toggleChar(toggleChar),
                                                      _btn_position(btn_position),
                                                      _lmp_position(lamp_position),
                                                      _lightmode(lightmode){};

        SimplePushButton(char const toggleChar,
                         ButtonHandler::MatrixPosition const btn_position,
                         LampHandler::DriverPosition const lamp_position) : SimplePushButton(toggleChar, btn_position, lamp_position, LightMode::WhenDashboardIsLit){};

        SimplePushButton(char const toggleChar,
                         ButtonHandler::MatrixPosition const btn_position,
                         LightMode const lightmode) : SimplePushButton(toggleChar, btn_position, LampHandler::DriverPosition::None, lightmode){};

        SimplePushButton(char const toggleChar,
                         ButtonHandler::MatrixPosition const btn_position) : SimplePushButton(toggleChar, btn_position, LampHandler::DriverPosition::None, LightMode::WhenDashboardIsLit){};

        void setCurrentState(const uint8_t button, const bool state, const bool prev_state);
        void registerWith(ButtonHandler &bh);
        void begin();

    protected:
    private:
        char _toggleChar;
        ButtonHandler::MatrixPosition _btn_position;
        LampHandler::DriverPosition _lmp_position;
        LightMode _lightmode;
        SimplePushButton() = delete;
        SimplePushButton(const SimplePushButton &) = delete;
        SimplePushButton &operator=(const SimplePushButton &) = delete;
    };
} // namespace BusDashboard