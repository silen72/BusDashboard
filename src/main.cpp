#include <Arduino.h>
#include <Keyboard.h>
#include <SPI.h>
#include "Dashboard.h"
#include "debug/LampTester.h"

using namespace BusDashboard;

ButtonHandler buttonHandler(LeonardoPins::BUTTON_MATRIX_CS, ButtonHandler::MCP23S17_ADRS);
LampHandler lampHandler(LeonardoPins::LAMP_DRIVER_SI, LeonardoPins::LAMP_DRIVER_SCK, LeonardoPins::LAMP_DRIVER_RCK);
KeyboardHandler keyboardHandler;
KomsiHandler komsiHandler;
CANBus canBus(LeonardoPins::CAN_CS, LeonardoPins::CAN_NT);
Dashboard dashboard(LeonardoPins::POWER_RELAY);

LampTester debuglamptester(lampHandler);

void setup()
{
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in: KOMSI)
  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)

  buttonHandler.begin();
  canBus.begin();
  keyboardHandler.begin();
  lampHandler.begin();
  dashboard.begin();      // wake up the dashboard (enable power for the transformers)
  lampHandler.allOff();   // switch off all lamps (a flaw in hw design: the lamps got power with the dashboard.begin() instruction, so they probably already have flashed...)

}

#ifdef SerialDebug
#warning Compiling with Serial debugging enabled - connect the arduino to a Serial Monitor, not KOMSI
#warning to configure Serial debugging use the build_flags = -D SerialDebug line in platformio.ini
#endif

uint8_t lampPosition = 0;
uint8_t prevpos = lampPosition + 1;

unsigned long m = millis();

void loop()
{
  if (Serial.available()) {
    int val = Serial.read();
    komsiHandler.processIncoming(val);
  }

  // moving lights for lamp 1 to 4 (0 ..3); next every second
  if ((millis() - m) > 1000UL) {
    lampHandler.setState(lampPosition, true);
    lampHandler.setState(prevpos, false);
    prevpos = lampPosition;
    lampPosition++;
    if (lampPosition >= 4) lampPosition = 0;
  }

  buttonHandler.scan();
  lampHandler.update();
  keyboardHandler.update();
  dashboard.checkIdle();

}
