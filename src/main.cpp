#include <Arduino.h>
#include <Keyboard.h>
#include <SPI.h>
#include "dashboard/Dashboard.h"
#include "lampdriver/LampHandler.h"

using namespace BusDashboard;

Dashboard &dashboard = Dashboard::instance();

void setup()
{
  SPI.begin();          // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200); // initialize communication with the PC (only in: KOMSI)
  Keyboard.begin();     // initialize communication with the PC (only out: Keyboard)
  dashboard.begin();    // wake up the dashboard (enable power for the transformers)
  pinMode(13, OUTPUT);  // debugging: onboard blinks
}

#ifdef SerialDebug
#warning Compiling with Serial debugging enabled - connect the arduino to a Serial Monitor, not KOMSI
#warning to configure Serial debugging use the build_flags = -D SerialDebug line in platformio.ini
#endif

void loop()
{
  dashboard.update();

  // debugging:

  const unsigned long seconds = millis() / 1000;
  const bool state = (seconds % 2 == 0);
  digitalWrite(13, state);
}
