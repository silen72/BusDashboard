#pragma once
#include "KeyPress.h"
#include "KeyRelease.h"

namespace BusDashboard {
    class KeyPressRelease : public KeyPress {
    private:
        const uint16_t _duration;
        KeyPressRelease(const KeyPressRelease&) = delete;
        KeyPressRelease& operator=(const KeyPressRelease&) = delete;
    public:
        KeyPressRelease(const uint8_t keycode, const uint16_t duration = KeyAction::DEFAULT_KEYPRESS_DURATION_MS);
    };
}