#include "dashboard/SimplePushButton.h"
#include "buttonmatrix/ButtonHandler.h"
#include "dashboard/Dashboard.h"
#include "keyboard/KeyboardHandler.h"

namespace BusDashboard
{

    void SimplePushButton::setCurrentState(const uint8_t button, const bool state, const bool prev_state)
    {
        if (state != prev_state)
        {
            if (_holdKey)
            {
                if (state)
                    Dashboard::instance().keyboardHandler().addPressAction(_toggleChar);
                else
                    Dashboard::instance().keyboardHandler().addReleaseAction(_toggleChar);
            }
            else if (state)
                Dashboard::instance().keyboardHandler().addPressReleaseAction(_toggleChar);
        }
        if (_lmp_position != LampHandler::DriverPosition::None)
        {
            bool lightOn;
            switch (_lightmode)
            {
            case LightMode::AlwaysOn:
                lightOn = true;
                break;
            case LightMode::WhenDashboardIsLit:
                lightOn = Dashboard::instance().isLit();
                break;
            case LightMode::WhenOff:
                lightOn = !state;
                break;
            case LightMode::WhenOn:
                lightOn = state;
                break;
            default:
                lightOn = false;
                break;
            }
            Dashboard::instance().lampHandler().setState(_lmp_position, lightOn);
        }
    }

    void SimplePushButton::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, _btn_position);
    }

} // namespace BusDashboard
