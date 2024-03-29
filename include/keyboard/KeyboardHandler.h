#pragma once
#include <Arduino.h>
#include "ItemNode.h"
#include "keyboard/KeyAction.h"

namespace BusDashboard
{

    class Dashboard;

    /**
    handles the keystrokes that need to be sent to the PC
    */
    class KeyboardHandler
    {

    public:
        enum class Keys : uint8_t
        {
            KEYPAD_0 = 234,
            KEYPAD_1 = 225,
            KEYPAD_2 = 226,
            KEYPAD_3 = 227,
            KEYPAD_4 = 228,
            KEYPAD_5 = 229,
            KEYPAD_6 = 230,
            KEYPAD_7 = 231,
            KEYPAD_8 = 232,
            KEYPAD_9 = 233,
            KEYPAD_ASTERISK = 221,
            KEYPAD_ENTER = 224,
            KEYPAD_MINUS = 222,
            KEYPAD_PERIOD = 235,
            KEYPAD_PLUS = 223,
            KEYPAD_SLASH = 220,
            KEY_BACKSLASH = 185,
            KEY_COMMA = 190,
            KEY_ENTER = 176,
            KEY_EQUAL = 182,
            KEY_LEFT_BRACE = 183,
            KEY_MENU = 237,
            KEY_MINUS = 181,
            KEY_NON_US_NUM = 186,
            KEY_NUM_LOCK = 219,
            KEY_PAUSE = 208,
            KEY_PERIOD = 191,
            KEY_PRINTSCREEN = 206,
            KEY_QUOTE = 188,
            KEY_RIGHT_BRACE = 184,
            KEY_SCROLL_LOCK = 207,
            KEY_SEMICOLON = 187,
            KEY_SLASH = 192,
            KEY_SPACE = 180,
            KEY_TILDE = 189
        };

        KeyboardHandler() {}

        /**
         * adds a generic keyboard action to the keyboard queue
         * @param[in] action: the action you want to add
         */
        void addAction(KeyAction &action);

        /**
        * adds a press keyboard action to the keyboard queue
        * @param[in] keycode: the key to press
        */
        void addPressAction(const uint8_t keycode);

        /**
        * adds a release keyboard action to the keyboard queue
        * @param[in] keycode: the key to release
        */
        void addReleaseAction(const uint8_t keycode);

        /**
        * adds a combined press and release keyboard action to the keyboard queue
        * @param[in] keycode: the key to press and release
        * @param[in] duration: the duration between pressing and releasing (optional, defaults to DEFAULT_KEYPRESS_DURATION_MS)
        */
        void addPressReleaseAction(const uint8_t keycode, const unsigned long duration = KeyAction::DEFAULT_KEYPRESS_DURATION_MS);

        /**
        * adds a combined press and release with modifier keyboard action to the keyboard queue
        * @param[in] keycode: the key to press and release
        * @param[in] duration: the duration between pressing and releasing (optional, defaults to DEFAULT_KEYPRESS_DURATION_MS)
        */
        void addPressReleaseActionMod(const uint8_t keycode, const uint8_t modifierkeycode, const unsigned long duration = KeyAction::DEFAULT_KEYPRESS_DURATION_MS);

        /**
         * @returns the time past (in ms) since the last keystroke has been sent to the PC
         */
        unsigned long idleTime() const { return millis() - _lastKeyActionMs; }

        /**
         * updates the keyboard actions in the queue (calls their run() and removes them, when they are both done() and removable())
         */
        void update(const uint64_t timestamp);

        /**
         * initializes the instance
         */
        void begin();

    protected:
    private:
        ItemNode<KeyAction> *_nextActionNode = nullptr;
        unsigned long _lastKeyActionMs = UINT32_MAX; // timestamp of the last key action

        // disallow creation
        KeyboardHandler(const KeyboardHandler &) = delete;
        KeyboardHandler &operator=(const KeyboardHandler &) = delete;
    };

} // namespace BusDashboard
