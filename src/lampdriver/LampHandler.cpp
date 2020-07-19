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

    LampHandler::LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch, const uint8_t numberOfICs):
        _pinSerial(pinSerial),
        _pinSerialClock(pinSerialClock),
        _pinLatch(pinLatch),
        _numberOfIcs(std::min(numberOfICs, (uint8_t)32)) // no more than 32 ICs allowed
    {
        // initialize all possible members of the lamps vector
        _lamps.reserve(_numberOfIcs * 8);
        for (size_t i=0; i < _lamps.capacity(); i++) _lamps[i] = nullptr;
        // initial state of all lamps: off (both logical and hardware)
        _lampstates.assign(_numberOfIcs, 0);
        _lampstatesHw.assign(_numberOfIcs, 0);
        // initialize all Lamp instances
        for (uint8_t position = 0; position <= _numberOfIcs * 8 - 1; position++) {
             _lamps[position] = new Lamp(*this, position);
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
        while ((doWrite == false) && (idx < _numberOfIcs));

        if (!doWrite) return;
        
        digitalWrite(_pinLatch, LOW);
        for (idx = 0; idx < _numberOfIcs; idx++) {
            shiftOut(_pinSerial, _pinSerialClock, MSBFIRST, _lampstates[idx]);
            _lampstatesHw[idx] = _lampstates[idx];
        }
        digitalWrite(_pinLatch, HIGH);
    }

    bool Lamp::state() const { return _parent.state(_position); }
    bool Lamp::hwstate() const { return _parent.state(_position, true); }
    void Lamp::setState(const bool state) { _parent.setState(_position, state); }
}