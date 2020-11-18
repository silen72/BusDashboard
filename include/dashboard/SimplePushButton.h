#pragma once
#include "buttonmatrix/ButtonHandler.h"
#include "buttonmatrix/ButtonListener.h"
#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    class SimplePushButton : public ButtonListener
    {
    public:
        enum class LightMode
        {
            AlwaysOn,
            AlwaysOff,
            WhenOn,
            WhenOff,
            WhenDashboardIsLit,
            Ignore
        };

        SimplePushButton(uint8_t const toggleChar,
                         ButtonHandler::MatrixPosition const btn_position,
                         LampHandler::DriverPosition const lamp_position = LampHandler::DriverPosition::None,
                         LightMode const lightmode = LightMode::WhenDashboardIsLit,
                         bool const holdKey = false) : _toggleChar(toggleChar),
                                                       _btn_position(btn_position),
                                                       _lmp_position(lamp_position),
                                                       _lightmode(lightmode), _holdKey(holdKey){};

        void setCurrentState(const uint8_t button, const bool state, const bool prev_state) override;
        void registerWith(ButtonHandler &bh) override;
        void begin() override {};

    protected:
    private:
        uint8_t _toggleChar;
        ButtonHandler::MatrixPosition _btn_position;
        LampHandler::DriverPosition _lmp_position;
        LightMode _lightmode;
        bool _holdKey;
        SimplePushButton() = delete;
        SimplePushButton(const SimplePushButton &) = delete;
        SimplePushButton &operator=(const SimplePushButton &) = delete;
    };
} // namespace BusDashboard