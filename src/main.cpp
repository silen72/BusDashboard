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

  unsigned long debug_start = millis();
  while (!Serial && ((millis () - debug_start) <= 5000));

  while (!Serial.available()) {}
  while (Serial.available()) { Serial.read(); }

  Serial.println(F("start"));
  while (!Serial.available()) {}
  while (Serial.available()) { Serial.read(); }
  

  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)
  Serial.println(F("keyboard init done"));
  while (!Serial.available()) {}
  while (Serial.available()) { Serial.read(); }

  BusDashboard::Dashboard::begin(); // wake up the dashboard
  Serial.println(F("dashboard init done"));
  Serial.read();

  /*
  keyboardHandler = new KeyboardHandler();
  Serial.println("keyboardHandler init done");
  while (!Serial.available()) {}
  while (Serial.available()) { Serial.read(); }
  */
}

uint32_t t = 0;

void loop() {
  if (Serial.available()) {
    int val = Serial.read();
    komsiHandler.processIncoming(val);
    Serial.println(F("komsiHandler done"));
  }
  buttonHandler.scan();
  Serial.println(F("buttonHandler done"));
  lampHandler.update();
  Serial.println(F("lampHandler done"));
  keyboardHandler.update();
  Serial.println(F("keyboardHandler done"));
  BusDashboard::Dashboard::checkIdle();
  Serial.println(F("checkIdle done"));
  delay(750);
  Serial.println(t++);
  Serial.println(F(" - loop done"));
}
