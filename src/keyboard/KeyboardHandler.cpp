#include "keyboard/KeyboardHandler.h"
#include "keyboard/KeyAction.h"
#include "keyboard/KeyPress.h"
#include "keyboard/KeyRelease.h"
#include "keyboard/KeyPressRelease.h"
#include "keyboard/KeyPressReleaseMod.h"

namespace BusDashboard {
	KeyboardHandler* KeyboardHandler::_instance = 0;

	void KeyboardHandler::addPressAction(const uint8_t keycode)
	{
		addAction(*(new KeyPress(keycode)));
	}

	void KeyboardHandler::addReleaseAction(const uint8_t keycode)
	{
		addAction(*(new KeyRelease(keycode)));
	}

	void KeyboardHandler::addPressReleaseAction(const uint8_t keycode, const uint16_t duration)
	{
		addAction(*(new KeyPressRelease(keycode, duration)));
	}

	void KeyboardHandler::addPressReleaseActionMod(const uint8_t keycode, const uint8_t modifierkeycode, const uint16_t duration)
	{
		addAction(*(new KeyPressReleaseMod(keycode, modifierkeycode, duration)));
	}

	void KeyboardHandler::update() {
		if (_keyactions.size() == 0) return;
		KeyAction* ka = _keyactions.front();
		ka->run();
		if (ka->removable()) {
			_keyactions.pop();
			delete ka;
			ka = nullptr;
			_lastKeyActionMs = millis();
		}
	}
}