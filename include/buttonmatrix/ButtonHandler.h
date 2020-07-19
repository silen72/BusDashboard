#pragma once
#include <SPI.h>
#include <bitset>
#include <gpio_MCP23S17.h>	// install library "gpio_MCP23S17 by sumotoy" in PIO
#include "ButtonListener.h"

namespace BusDashboard {

	class ButtonListener; // forward declaration

	/**
	handles the multiplexer matrix used to query up to 64 button states
	*/
	class ButtonHandler {
	public:

        /**
         * enum of all buttons on the dashboard with a mapping to their position on the button matrix
		 * note: sorry for the missing translation, I may add it sometimes ...
         */
		enum MatrixPositions {
			Asr,
			Blinker_Links,
			Blinker_Rechts,
			Blinker_Warn,
			Licht_Abblend,
			Licht_Fahrer,
			Licht_Fern,
			Licht_Innen_1,
			Licht_Innen_2,
			Licht_Nebelschluss,
			Licht_Standlicht,
			Gangschaltung_D,
			Gangschaltung_N,
			Gangschaltung_R,
			Haltestellenansage,
			Haltestellenbremse,
			Hupe,
			Kindersteuerung,
			Kneeling_1,
			Kneeling_2,
			Parkbremse,
			Profil_Wechseln,
			Retarder,
			Scheibenwischer_1,
			Scheibenwischer_2,
			Scheibenwischer_3,
			Scheinwerfer_A1,
			Scheinwerfer_A2,
			Scheinwerfer_I1,
			Scheinwerfer_I2,
			Tuer_1,
			Tuer_2,
			Tuer_3,
			Tuer_Freigabe,
			Tuer_Sperren,
			Wischwasser,
			Zuendung_1,
			Zuendung_2,
			Zuendung_3
		};

		static const uint8_t MATRIX_ROW_COUNT = 8; // the number of rows in the multiplexer matrix
		static const uint8_t MATRIX_COLUMN_COUNT = 8; // the number of columns in the multiplexer matrix
		static const uint8_t BUTTON_COUNT = MATRIX_ROW_COUNT * MATRIX_COLUMN_COUNT; // the number of buttons connected to the multiplexer matrix
		static const uint8_t MCP23S17_ADRS = 0x20; // SPI: A0 .. A2 to ground = 0x20
		static const uint8_t READ_DELAY_MS = 12; // delay between two row reads -> 8 rows => each button gets read every ~100ms

		/**
		 * add listeners to specific buttons
		 * the listeners get notified of the current state each time a button is scanned
		 * @param[in] listener the listener instance to add
		 * @param[in] button the button the listener wants to be informed about (button number on the scan matrix)
		*/
		bool addListener(ButtonListener& listener, const uint8_t button);

		/**
		 * queries the current state of connected buttons and notifies listeners
		 * this needs to be called repeatedly - note that one only queries one row of buttons
		 * note also that this neither tries to debounce buttons nor does it detect changes, the listeners receive 'raw' data
		*/
		void scan();

		ButtonHandler(const uint8_t pin_cs, const uint8_t address);

	protected:

	private:
        const uint8_t _pin_cs; // SPI: the Arduino pin the button matrix CS is connected to
		const uint8_t _address = 0x20; // SPI: A0 .. A2 connected to ground

		ButtonListenerNode* _listener[BUTTON_COUNT]; // registered listeners
		uint8_t _row = 0; // the current row (this row gets queried on updateStatus)
		uint32_t _lastButtonChange = 0; // timestamp: last change in a button state
		uint32_t _lastRead = 0;  // timestamp: last read action (for throtteling the read frequency)

		gpio_MCP23S17* _mcp;

		/**
		 * reads the pins on the input IC of the muxer (GPIO_Bx)
         * @returns the read pin state, a bit array of eight bits; a set bit means the pin is connected -> button is pressed
		 */
		uint8_t readMuxColumns() {
			return _mcp->gpioRegisterReadByte(MCP23S17_GPIO + 1);
		}

		/**
		 * sets the output pin for the current row on the output IC of the muxer (GPIO_Ax)
		 */
		void setMuxRow() {
			const uint8_t pin = 1 << _row;
			_mcp->gpioRegisterWriteByte(MCP23S17_GPIO, pin);
		}

		/**
		 * notifies all listeners of the current value for a button (only to be called from within updateStatus())
         * @param[in] button this button has been read
         * @param[in] state this is the state that has been read
		 */
		void notify(const uint8_t button, const bool state);

        ButtonHandler() = delete;
        ButtonHandler(const ButtonHandler&) = delete;
        ButtonHandler& operator=(const ButtonHandler&) = delete;

	};
}
