#pragma once
#include <Arduino.h>

namespace BusDashboard {

	class ButtonHandler;

	/**
	interface for classes that want to receive button states
	*/
	class ButtonListener
	{
	public:
		/**
		this gets called every time a button state is read from the muxer hardware
		@param[in] button the button that has been read
		@param[in] state the state the button is in (true: connected / pressed)
		@param[in] state the state the button was in last time it was read
		@returns true, if the state has changed
		*/
		virtual void setCurrentState(const uint8_t button, const bool state, const bool prev_state) = 0;
		virtual void registerWith(ButtonHandler &bh) = 0;
		virtual void begin() = 0;
	};
}