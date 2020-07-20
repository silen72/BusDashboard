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

void setup() {
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in:KOMSI)

  pinMode(ArduinoPins::BUTTON_MATRIX_CS, OUTPUT);
  pinMode(ArduinoPins::CAN_CS, OUTPUT);
  pinMode(ArduinoPins::CAN_NT, OUTPUT);
  pinMode(ArduinoPins::LAMP_DRIVER_RCK, OUTPUT);
  pinMode(ArduinoPins::LAMP_DRIVER_SCK, OUTPUT);
  pinMode(ArduinoPins::LAMP_DRIVER_SI, OUTPUT);
  pinMode(ArduinoPins::MAINS_RELAY, OUTPUT);

  digitalWrite(ArduinoPins::MAINS_RELAY, LOW);

  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)
  BusDashboard::Dashboard::begin(); // wake up the dashboard
}

void loop() {
  if (Serial.available()) {
    int val = Serial.read();
    komsiHandler.processIncoming(val);
  }
  buttonHandler.scan();
  lampHandler.update();
  keyboardHandler.update();
  BusDashboard::Dashboard::checkIdle();
}
