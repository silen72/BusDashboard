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
}