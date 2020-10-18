#include "keyboard/KeyPress.h"

namespace BusDashboard
{
    void KeyPress::doAction()
    {
#ifdef SerialDebug
        Serial.print(F("KeyPress::doAction(), code: "));
        Serial.println((int)_keycode);
#else
        Keyboard.press(_keycode);
#endif
    }
} // namespace BusDashboard