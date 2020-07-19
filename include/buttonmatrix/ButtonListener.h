#pragma once
#include <Arduino.h>

namespace BusDashboard {
	/**
	interface for classes that want to receive button states
	*/
	class ButtonListener {
	public:
		/**
		this gets called every time a button state is read from the muxer hardware
		@param[in] button the button that has been read
		@param[in] state the state the button was in (true: connected / pressed)
		*/
		virtual void setCurrentState(const uint8_t button, const bool state) = 0;
	};

	    class ButtonListenerNode {
        public:
            ButtonListenerNode* next() { return _next; }
            ButtonListenerNode* append(ButtonListener& buttonlistener) {
				_next = new ButtonListenerNode(buttonlistener);
				return _next;
			}
            ButtonListener* item() { return _item; }
            ButtonListenerNode(ButtonListener& buttonlistener):_item(&buttonlistener), _next(nullptr) {};
        
        private:
            ButtonListener* _item;
            ButtonListenerNode* _next;

            ButtonListenerNode() = delete;
            ButtonListenerNode(const ButtonListenerNode&) = delete;
            ButtonListenerNode& operator=(const ButtonListenerNode&) = delete;

    };
}