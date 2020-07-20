#include "keyboard/KeyboardHandler.h"
#include "keyboard/KeyAction.h"
#include "keyboard/KeyPress.h"
#include "keyboard/KeyRelease.h"
#include "keyboard/KeyPressRelease.h"
#include "keyboard/KeyPressReleaseMod.h"

namespace BusDashboard {

	void KeyboardHandler::addAction(KeyAction& action) {
		if (nullptr == _nextActionNode) {
			_nextActionNode = new ItemNode<KeyAction>(action);
		} else {
			_nextActionNode->append(action);
		}
	}

	void KeyboardHandler::addPressAction(const uint8_t keycode)
	{
		addAction(*(new KeyPress(keycode)));
	}

	void KeyboardHandler::addReleaseAction(const uint8_t keycode)
	{
		addAction(*(new KeyRelease(keycode)));
	}

	void KeyboardHandler::addPressReleaseAction(const uint8_t keycode, const unsigned long duration)
	{
		addAction(*(new KeyPressRelease(keycode, duration)));
	}

	void KeyboardHandler::addPressReleaseActionMod(const uint8_t keycode, const uint8_t modifierkeycode, const unsigned long duration)
	{
		addAction(*(new KeyPressReleaseMod(keycode, modifierkeycode, duration)));
	}

	void KeyboardHandler::update() {
		ItemNode<KeyAction> *node = _nextActionNode;
		if (nullptr == node) return;
		KeyAction* ka = node->item();
		ka->run();
		if (ka->removable()) {
			// remove action and node 
			node = _nextActionNode->next();
			delete ka;
			delete _nextActionNode;
			_nextActionNode = node;
			_lastKeyActionMs = millis();
		}
	}

	void KeyboardHandler::begin(){};
}