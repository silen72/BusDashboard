#include "keyboard/KeyPress.h"

namespace BusDashboard {
    void KeyPress::doAction() {
        Keyboard.press(_keycode);
    }
}