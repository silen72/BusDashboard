#include <Arduino.h>
#include <stdint.h>
#include <Keyboard.h>
#include <SPI.h>

void setup() {
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in:KOMSI)
  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)
}

void loop() {

}