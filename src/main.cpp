#include <Arduino.h>
#include <stdint.h>
#include <Keyboard.h>
#include <SPI.h>

// Pins used to connect the shift registers
enum ArduinoPins {
  LAMP_DRIVER_SI = 4,
  LAMP_DRIVER_RCK = 5,
  LAMP_DRIVER_SCK = 6,
  MAINS_RELAY = 7,
  CAN_CS = 8,             // CAN-Bus driver (SPI)
  CAN_NT = 9,             // CAN-Bus driver (SPI)
  BUTTON_MATRIX_CS = 10,  // button matrix (SPI)
  SPI_MOSI = 11,          // on a leonardo board this is only available on the ISCP header (ISCP-4)!
  SPI_MISO = 12,          // on a leonardo board this is only available on the ISCP header (ISCP-1)!
  SPI_SCK = 13            // on a leonardo board this is only available on the ISCP header (ISCP-3)!
};

void setup() {
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in:KOMSI)
  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)
}

void loop() {

}