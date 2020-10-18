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
        if (_lmp_position > 0)
            Dashboard::instance().lampHandler().setState(_lmp_position, Dashboard::instance().isLit());
    }

    void SimpleSwitch::registerWith(ButtonHandler &bh)
    {
        bh.addListener(*this, _btn_position);
    }

    void SimpleSwitch::begin()
    {}

} // namespace BusDashboard

