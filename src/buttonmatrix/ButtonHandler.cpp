#include "dashboard/Dashboard.h"
#include "buttonmatrix/ButtonListener.h"
#include "buttonmatrix/ButtonHandler.h"

namespace BusDashboard
{
  ButtonHandler::ButtonHandler(const uint8_t pin_cs, const uint8_t address) : _pin_cs(pin_cs), _address(address), _mcp(new gpio_MCP23S17(pin_cs, _address))
  {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
      _listener[i] = nullptr;
      _prevState[i] = false;
    }
  }

  bool ButtonHandler::previousState(const uint8_t button)
  {
    return _prevState[button];
  }

  bool ButtonHandler::addListener(ButtonListener &listener, const MatrixPosition button)
  {
    uint8_t index = (uint8_t)button;
    if (index >= BUTTON_COUNT)
      return false;
#ifdef SerialDebug
    Serial.print(F("ButtonHandler::addListener, button "));
    Serial.print((int)button);
#endif
    if (nullptr == _listener[index])
    {
      _listener[index] = new ItemNode<ButtonListener>(listener);
#ifdef SerialDebug
      Serial.print(F(" created root node"));
      Serial.println((int)_listener[index]);
#endif
    }
    else
    {
#ifdef SerialDebug
      Serial.print(F(" appends node to "));
      Serial.println((int)_listener[index]);
#endif
      _listener[index]->append(listener);
    }
    return true;
  }

  void ButtonHandler::notify(const uint8_t button, const bool state)
  {
    bool prevstate = _prevState[button];
    if (state != prevstate)
    {
      Dashboard::instance().resetIdleTimer(); // a button state has changed -> Dashboard has been used
      _prevState[button] = state;
    }
    // notify listeners
    ItemNode<ButtonListener> *node = _listener[button];
    while (nullptr != node)
    {
      node->item()->setCurrentState(button, state, prevstate);
      node = node->next();
    }
  }

  void ButtonHandler::begin()
  {
    _mcp->begin();
    //setup the gpio
    /*        7     6     5	     4     3   2     1    0
  IOCON = BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL -NC-
  
  bit 7 BANK: Controls how the registers are addressed
    1 = The registers associated with each port are separated into different banks.
    0 = The registers are in the same bank (addresses are sequential).
  bit 6 MIRROR: INT Pins Mirror bit
    1 = The INT pins are internally connected
    0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
  bit 5 SEQOP: Sequential Operation mode bit
    1 = Sequential operation disabled, address pointer does not increment.
    0 = Sequential operation enabled, address pointer increments.
  bit 4 DISSLW: Slew Rate control bit for SDA output
    1 = Slew rate disabled
    0 = Slew rate enabled
  bit 3 HAEN: Hardware Address Enable bit (MCP23S17 only) (Note 1)
    1 = Enables the MCP23S17 address pins.
    0 = Disables the MCP23S17 address pins.
  bit 2 ODR: Configures the INT pin as an open-drain output
    1 = Open-drain output (overrides the INTPOL bit.)
    0 = Active driver output (INTPOL bit sets the polarity.)
  bit 1 INTPOL: This bit sets the polarity of the INT output pin
    1 = Active-high
    0 = Active-low
  bit 0 Unimplemented: Read as ‘0’
  */

    // left in case of trouble with SPI: might need to slow down SPI in the future ...
    //SPI.setClockDivider(SPI_CLOCK_DIV32);

    _mcp->gpioRegisterWriteByte(MCP23S17_IOCON, 0b00000000);
    _mcp->gpioRegisterWriteByte(MCP23S17_IODIR, 0x00);     // IODIRA: all output -> row driver
    _mcp->gpioRegisterWriteByte(MCP23S17_IODIR + 1, 0xff); // IODIRB: all input -> column query
    _mcp->gpioRegisterWriteByte(MCP23S17_GPPU + 1, 0x00);  // no pullup on input ports
  }

  void ButtonHandler::scan()
  {
    // throttle reads
    const uint32_t now = millis();
    if (abs(now - _lastRead) < READ_DELAY_MS)
      return;
    // read one row of buttons
    setMuxRow();
    const uint8_t button = _row * MATRIX_COLUMN_COUNT;
    uint8_t result = readMuxColumns();
    // calculate button position and notify corresponding listeners
    for (uint8_t bit = 0; bit < MATRIX_COLUMN_COUNT; bit++)
    {
      notify(button + bit, (result & 0b10000000 ? 1 : 0));
      result = result << 1;
    }
    // prepare for next call
    _row++;
    if (_row >= MATRIX_ROW_COUNT)
      _row = 0;
    _lastRead = now;
  }
} // namespace BusDashboard