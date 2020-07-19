#include "keyboard/KeyPressRelease.h"

namespace BusDashboard {
    KeyPressRelease::KeyPressRelease(const uint8_t keycode, const uint16_t duration) : KeyPress(keycode), _duration(duration) {
        KeyRelease* kr = new KeyRelease(_keycode);
        kr->waitFor(*this, duration);
    }
}