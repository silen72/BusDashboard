#include "keyboard/KeyRelease.h"

namespace BusDashboard {
    void KeyRelease::doAction() {
        Keyboard.release(_keycode);
    }
}