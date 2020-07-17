#pragma once
#include "KeyPress.h"
#include "KeyRelease.h"

namespace BusDashboard {
    class KeyPressReleaseMod : public KeyPress {
    private:
        const uint16_t _duration;
        KeyPressReleaseMod(const KeyPressReleaseMod&) = delete;
        KeyPressReleaseMod& operator=(const KeyPressReleaseMod&) = delete;
    public:
        KeyPressReleaseMod(const uint8_t keycode, const uint8_t modifierkeycode, const uint16_t duration = KeyboardHandler::DEFAULT_KEYPRESS_DURATION_MS);
    };
}
