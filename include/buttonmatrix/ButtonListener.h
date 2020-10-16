#pragma once
#include <Arduino.h>
#include "buttonmatrix/ButtonHandler.h"

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
		@param[in] state the state the button was in (true: connected / pressed)
		@returns true, if the state has changed
		*/
		virtual bool setCurrentState(const uint8_t button, const bool state) = 0;
		virtual void registerWith(ButtonHandler &bh) = 0;
	};
}