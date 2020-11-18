#pragma once
#include "KeyAction.h"

namespace BusDashboard {
    class KeyPress : public KeyAction {
    private:
        KeyPress(const KeyPress&) = delete;
        KeyPress& operator=(const KeyPress&) = delete;
    protected:
        const uint8_t _keycode;
    public:
        KeyPress(const uint8_t keycode) :_keycode(keycode) {}
        void doAction() override;
    };
}