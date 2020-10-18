#include "lampdriver/LampHandler.h"

namespace BusDashboard
{
    void LampHandler::setState(const uint8_t position, const bool state)
    {
        if (!isValidPosition(position))
            return;
        uint8_t index;
        uint8_t bitmask;
        uint8_t newstate;
        calculateIndexAndBitmask(position, index, bitmask);
        if (state)
        {
            newstate = _lampstates[index] | bitmask;
        }
        else
        {
            newstate = _lampstates[index] & (0xff ^ bitmask);
        }
        if (newstate != _lampstates[index])
        {
#ifdef SerialDebug
            Serial.print(F("LampHandler::setState: position: "));
            Serial.print((int)position);
            Serial.print(F(", state: "));
            Serial.print(state ? "on; " : "off; ");
            Serial.print(F(", index: "));
            Serial.print((int)index);
            Serial.print(F(", bitmask: "));
            Serial.print(bitmask, BIN);
            Serial.print(F(", "));
            Serial.print((int)_lampstates[index]);
            Serial.print(F(" -> "));
            Serial.println((int)newstate);
#endif
            _lampstates[index] = newstate;
        }
    }

    void LampHandler::update()
    {
        uint64_t const now = millis();
        int64_t const diff = abs(now - _lastWriteMs);
        if ((uint64_t)diff < WRITE_DELAY)
            return;
        writeLampState();
        _lastWriteMs = now;
    }

    bool LampHandler::state(const uint8_t position, const bool queryHw) const
    {
        if (!isValidPosition(position))
            return false;
        uint8_t index;
        uint8_t bitmask;
        calculateIndexAndBitmask(position, index, bitmask);
        uint8_t const value = (queryHw ? _lampstatesHw[index] : _lampstates[index]);
        return ((value & bitmask) > 0);
    }

    LampHandler::LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch) : _pinSerial(pinSerial), _pinSerialClock(pinSerialClock), _pinLatch(pinLatch)
    {
        // initial state of all lamps: off (both logical and hardware)
        for (uint8_t i = 0; i < NUMBER_OF_ICS; i++)
        {
            _lampstates[i] = 0;
            _lampstatesHw[i] = 0;
        }
    }

    void LampHandler::writeLampState()
    {
        // is a shift out necessary (has any lamp state changed?)
        if (needsHwUpdate())
            writeToHw();
    }

    bool LampHandler::needsHwUpdate() const
    {
        for (uint8_t idx = 0; idx < NUMBER_OF_ICS; idx++)
        {
            if (_lampstatesHw[idx] != _lampstates[idx])
                return true;
        }
        return false;
    }

    void LampHandler::writeToHw()
    {
#ifdef SerialDebug
        Serial.println(F("LampHandler::writeLampState: shift out"));
#endif
        digitalWrite(_pinLatch, LOW);
        for (int8_t idx = NUMBER_OF_ICS - 1; idx >= 0; idx--)
        { // must be int8_t instead of uint_8 due to counting down to 0 (-1 actually)
            shiftOut(_pinSerial, _pinSerialClock, MSBFIRST, _lampstates[idx]);
#ifdef SerialDebug
            Serial.print((int)idx);
            Serial.print((_lampstatesHw[idx] == _lampstates[idx]) ? F(": ") : F("! "));
            Serial.print((int)_lampstatesHw[idx]);
            Serial.print(F(" <-> "));
            Serial.println(_lampstatesHw[idx], BIN);
#endif
            _lampstatesHw[idx] = _lampstates[idx];
        }
        digitalWrite(_pinLatch, HIGH);
    }

    void LampHandler::allOff()
    {
        for (int8_t idx = NUMBER_OF_ICS - 1; idx >= 0; idx--)
        {
            _lampstates[idx] = 0;
            _lampstatesHw[idx] = 0;
        }
        writeToHw();
    }

    void LampHandler::begin()
    {
        pinMode(_pinLatch, OUTPUT);
        pinMode(_pinSerial, OUTPUT);
        pinMode(_pinSerialClock, OUTPUT);
    }

    void LampHandler::calculateIndexAndBitmask(const uint8_t position, uint8_t &index, uint8_t &bitmask) const
    {
        /* 24V side:
           Position:  0   1   2   3   4   5   6   7    8   9  10  11  12  13  14  15   16  17  18  19  20  21  22  23
           IC:        --------      1      -------- |  --------      2      -------- |  --------      3      --------
           IC-Pin:   QH  QG  QF  QE  QD  QC  QB  QA | QH  QG  QF  QE  QD  QC  QB  QA | QH  QG  QF  QE  QD  QC  QB  QA
           index:     --------      0      -------- |  --------      1      -------- |  --------      2      --------
           bitnum:    7   6   5   4   3   2   1   0 |  7   6   5   4   3   2   1   0 |  7   6   5   4   3   2   1   0 

           5V side:
           Position: 47  46  45  44  43  42  41  40   39  38  37  36  35  34  33  32   31  30  29  28  27  26  25  24
           IC:        --------      1      -------- |  --------      2      -------- |  --------      3      --------
           IC-Pin:   QA  QB  QC  QD  QE  QF  QG  QH | QA  QB  QC  QD  QE  QF  QG  QH | QA  QB  QC  QD  QE  QF  QG  QH
           index:     --------      5      -------- |  --------      4      -------- |  --------      3      --------
           bitnum:    0   1   2   3   4   5   6   7 |  0   1   2   3   4   5   6   7 |  0   1   2   3   4   5   6   7 
        */

        index = position / 8;
        uint8_t const bitnum = 7 - position % 8;
        bitmask = 1 << bitnum;
    }
} // namespace BusDashboard