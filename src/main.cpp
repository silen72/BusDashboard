#include <Arduino.h>
#include <stdint.h>
#include <Keyboard.h>
#include <SPI.h>
#include "Dashboard.h"

using namespace BusDashboard;

ButtonHandler buttonHandler(BusDashboard::ArduinoPins::BUTTON_MATRIX_CS, BusDashboard::ButtonHandler::MCP23S17_ADRS);
LampHandler lampHandler(ArduinoPins::LAMP_DRIVER_SI, ArduinoPins::LAMP_DRIVER_SCK, ArduinoPins::LAMP_DRIVER_RCK);
KeyboardHandler keyboardHandler;
KomsiHandler komsiHandler;
CANBus canBus(ArduinoPins::CAN_CS, ArduinoPins::CAN_NT);
Dashboard dashboard(ArduinoPins::MAINS_RELAY);

void setup()
{
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in:KOMSI)
  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)

  buttonHandler.begin();
  canBus.begin();
  keyboardHandler.begin();
  lampHandler.begin();
  dashboard.begin();      // wake up the dashboard
}

void loop() {
  if (Serial.available()) {
    int val = Serial.read();
    komsiHandler.processIncoming(val);
  }
  buttonHandler.scan();
  lampHandler.update();
  keyboardHandler.update();
  dashboard.checkIdle();
}
