#include "keyboard/KeyRelease.h"

namespace BusDashboard {
    void KeyRelease::doAction() {
#ifdef SerialDebug
        Serial.print(F("KeyRelease::doAction(), code: "));
        Serial.println((int)_keycode);
#else
        Keyboard.release(_keycode);
#endif
    }
}