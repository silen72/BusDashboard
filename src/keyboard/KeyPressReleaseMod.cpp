#include "keyboard/KeyPressReleaseMod.h"
#include "keyboard/KeyRelease.h"

namespace BusDashboard
{
    KeyPressReleaseMod::KeyPressReleaseMod(const uint8_t keycode, const uint8_t modifierkeycode, const uint16_t duration) : KeyPress(modifierkeycode), _duration(duration)
    {
        auto *key = new KeyPress(keycode);
        key->waitFor(*this, 25);
        auto *keyr = new KeyRelease(keycode);
        keyr->waitFor(*key, duration);
        auto *modr = new KeyRelease(modifierkeycode);
        modr->waitFor(*keyr, 25);
    }
} // namespace BusDashboard