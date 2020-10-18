#include "dashboard/SimpleSwitch.h"
#include "buttonmatrix/ButtonHandler.h"
#include "dashboard/Dashboard.h"
#include "keyboard/KeyboardHandler.h"
#include "keyboard/KeyboardHandler.h"

namespace BusDashboard
{

    void SimpleSwitch::setCurrentState(const uint8_t button, const bool state, const bool prev_state)
    {
        if (state != prev_state)
        {
            if (state)
                Dashboard::instance().keyboardHandler().addPressReleaseAction(_sendOn);
            else
                Dashboard::instance().keyboardHandler().addPressReleaseAction(_sendOff);
        }
        if ((_lmp_position != LampHandler::DriverPosition::None) & (_lightmode != LightMode::Ignore))
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

    void SimpleSwitch::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, _btn_position);
    }

} // namespace BusDashboard
