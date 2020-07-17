#pragma once
#include <Arduino.h>
#include <queue>

namespace BusDashboard {

    class KeyAction;
    class KeyPressRelease;

    /**
    handles the keystrokes that need to be sent to the PC
    */
    class KeyboardHandler {

    private:
        static KeyboardHandler* _instance; // the singleton instance of KeyboardHandler
        std::queue<KeyAction*> _keyactions; // the queue of KeyAction
        uint32_t _lastKeyActionMs = UINT32_MAX; // timestamp of the last key action

        // disallow creation
        KeyboardHandler() {}
        KeyboardHandler(const KeyboardHandler&) = delete;
        KeyboardHandler& operator=(const KeyboardHandler&) = delete;

        ~KeyboardHandler() {}
        class CGuard {
        public:
            ~CGuard() {
                if (nullptr != KeyboardHandler::_instance) {
                    delete KeyboardHandler::_instance;
                    KeyboardHandler::_instance = nullptr;
                }
            }
        };

    public:
        enum Keys {
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
            KEYPAD_ASTERIX = 221,
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
            KEY_TILDE = 189,
            KEY_F7 = 200
        };

        static const uint16_t DEFAULT_KEYPRESS_DURATION_MS = 250;

        /**
         * @returns the singleton KeyboardHandler instance
         */
        static KeyboardHandler* instance() {
            static CGuard g;
            if (!_instance) _instance = new KeyboardHandler();
            return _instance;
        }

        /**
         * adds a generic keyboard action to the keyboard queue
         * @param[in] action: the action you want to add
         */
        void addAction(KeyAction& action) { _keyactions.push(&action); }

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
        void addPressReleaseAction(const uint8_t keycode, const uint16_t duration = DEFAULT_KEYPRESS_DURATION_MS);

        /**
        * adds a combined press and release with modifier keyboard action to the keyboard queue
        * @param[in] keycode: the key to press and release
        * @param[in] duration: the duration between pressing and releasing (optional, defaults to DEFAULT_KEYPRESS_DURATION_MS)
        */
        void addPressReleaseActionMod(const uint8_t keycode, const uint8_t modifierkeycode, const uint16_t duration = DEFAULT_KEYPRESS_DURATION_MS);

        /**
         * @returns the time past (in ms) since the last keystroke has been sent to the PC
         */
        uint32_t idleTime() const { return millis() - _lastKeyActionMs; }

        /**
         * updates the keyboard actions in the queue (calls their run() and removes them, when they are both done() and removable())
         */
        void update();

        /**
         * @returns the number of elements in the keyboard queue
         */
        size_t queuesize() const { return _keyactions.size(); }
    };
}