#include "lampdriver/LampHandler.h"

namespace BusDashboard {
	void LampHandler::setState(const uint8_t position, const bool state) {
        uint8_t const index = position / 8;
        uint8_t const bitnum = 7 - (position - index * 8);
        uint8_t const bitmask = 1 << bitnum;
        if (state) {
            _lampstates[index] |= bitmask;
        } else {
            _lampstates[index] &= (0xff ^ bitmask);
        }
	}

	void LampHandler::update() {
	}

	bool LampHandler::state(const uint8_t position, const bool queryHw) const {
        uint8_t const index = position / 8;
        uint8_t const bitnum = 7 - (position - index * 8);
        uint8_t const bitmask = 1 << bitnum;
        uint8_t const value = (queryHw ? _lampstatesHw[index] : _lampstates[index]);
        return ((value & bitmask) > 0);
	}

    LampHandler::LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch):
        _pinSerial(pinSerial),
        _pinSerialClock(pinSerialClock),
        _pinLatch(pinLatch)
    {
        // initial state of all lamps: off (both logical and hardware)
        for (uint8_t i = 0; i < NUMBER_OF_ICS; i++) {
            _lampstates[i] = 0;
            _lampstatesHw[i] = 0;
        }
    }

    void LampHandler::writeLampState() {
        // is a shift out necessary (has any lamp state changed?)
        bool doWrite = false;
        uint8_t idx = 0;
        do {
            doWrite = (_lampstatesHw[idx] != _lampstates[idx]) ;
            idx++;
        }
        while ((doWrite == false) && (idx < NUMBER_OF_ICS));

        if (!doWrite) return;
        
        digitalWrite(_pinLatch, LOW);
        for (idx = 0; idx < NUMBER_OF_ICS; idx++) {
            shiftOut(_pinSerial, _pinSerialClock, MSBFIRST, _lampstates[idx]);
            _lampstatesHw[idx] = _lampstates[idx];
        }
        digitalWrite(_pinLatch, HIGH);
    }

    void LampHandler::begin() {
        pinMode(_pinLatch, OUTPUT);
        pinMode(_pinSerial, OUTPUT);
        pinMode(_pinSerialClock, OUTPUT);
    }
}