#include "buttonmatrix/ButtonHandler.h"


namespace BusDashboard {
	bool ButtonHandler::addListener(ButtonListener& listener, const uint8_t button) {
		if (button >= ButtonHandler::BUTTON_COUNT) return false;
		_listener[button].push_back(&listener);
		return true;
	}

	void ButtonHandler::notify(const uint8_t button, const bool state) {
		if (_listener[button].size() == 0) return;
		for (auto bl : _listener[button]) bl->setCurrentState(button, state);
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
            std::bitset<8> x(result);
            notify(button + bit, (x.test(7) ? 1: 0));
            result = result << 1;
        }
        // prepare for next call
        _row++;
        if (_row >= MATRIX_ROW_COUNT) _row = 0;
        _lastRead = now;
	}
}	