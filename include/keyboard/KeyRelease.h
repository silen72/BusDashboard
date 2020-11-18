#pragma once
#include "KeyAction.h"

namespace BusDashboard {
    class KeyRelease : public KeyAction {
    private:
        const uint8_t _keycode;
        KeyRelease(const KeyRelease&) = delete;
        KeyRelease& operator=(const KeyRelease&) = delete;
    public:
        KeyRelease(const uint8_t keycode) :_keycode(keycode) {}
        void doAction() override;
    };
}