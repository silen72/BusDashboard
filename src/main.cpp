#include <Arduino.h>
#include <Keyboard.h>
#include <SPI.h>
#include "Dashboard.h"

#include "debug/LampTester.h"
#include "buttonmatrix/ButtonHandler.h"
#include "lampdriver/LampHandler.h"
#include "keyboard/KeyboardHandler.h"
#include "KOMSI/KOMSIHandler.h"
#include "CANBus/CANBus.h"

using namespace BusDashboard;

Dashboard dashboard(LeonardoPins::POWER_RELAY);

//LampTester debuglamptester(dashboard.lampHandler());

void setup()
{
  SPI.begin();            // initialize communication with the SPI devices (button matrix and CAN-bus driver)
  Serial.begin(115200);   // initialize communication with the PC (only in: KOMSI)
  Keyboard.begin();       // initialize communication with the PC (only out: Keyboard)
  dashboard.begin();      // wake up the dashboard (enable power for the transformers)
}

#ifdef SerialDebug
#warning Compiling with Serial debugging enabled - connect the arduino to a Serial Monitor, not KOMSI
#warning to configure Serial debugging use the build_flags = -D SerialDebug line in platformio.ini
#endif

//uint8_t lampPosition = 0;
//uint8_t prevpos = lampPosition + 1;
//unsigned long m = millis();

void loop()
{
  /*
  // moving lights for lamp 1 to 4 (0 ..3); next every second
  if ((millis() - m) > 1000UL) {
    dashboard.lampHandler().setState(lampPosition, true);
    dashboard.lampHandler().setState(prevpos, false);
    prevpos = lampPosition;
    lampPosition++;
    if (lampPosition >= 4) lampPosition = 0;
  }
  */

  dashboard.update();
}
