#include "buttonmatrix/ButtonHandler.h"


namespace BusDashboard {

    ButtonHandler::ButtonHandler(const uint8_t pin_cs, const uint8_t address):_pin_cs(pin_cs), _address(address), _mcp(new gpio_MCP23S17(pin_cs, _address)) {
        for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
            _listener[i] = nullptr;
        }
    }

	bool ButtonHandler::addListener(ButtonListener& listener, const uint8_t button) {
		if (button >= ButtonHandler::BUTTON_COUNT) return false;
        if (nullptr == _listener[button]) {
            _listener[button] = new ButtonListenerNode(listener);
        } else {
            _listener[button]->append(listener);
        }
		return true;
	}

	void ButtonHandler::notify(const uint8_t button, const bool state) {
        ButtonListenerNode* node = _listener[button];
        while (nullptr != node) {
            node->item()->setCurrentState(button, state);
            node = node->next();
        }
	}

	void ButtonHandler::scan() {
        // throttle reads
        const uint32_t now = millis();
        if (abs(now - _lastRead) < READ_DELAY_MS) return;
        // read one row of buttons
        setMuxRow();
        const uint8_t button = _row * MATRIX_COLUMN_COUNT;
        uint8_t result = readMuxColumns();
        // calculate button position and notify corresponding listeners
        for (uint8_t bit = 0; bit < MATRIX_COLUMN_COUNT; bit++) {
            notify(button + bit, (result & 0b10000000 ? 1: 0));
            result = result << 1;
        }
        // prepare for next call
        _row++;
        if (_row >= MATRIX_ROW_COUNT) _row = 0;
        _lastRead = now;
	}
}	