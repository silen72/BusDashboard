#pragma once
#include <Arduino.h>

namespace BusDashboard
{

   class LampHandler
   {

   public:
      static const uint8_t NUMBER_OF_ICS = 6; // the number of 74HC595 ICs on the board
      static const uint64_t WRITE_DELAY = 25; // wait at least this many ms between to actual writes to the lamp driver board

      enum DriverPosition
      {
         None = 0,
         // 24V: position 1 .. 24
         Gangschaltung,
         Tuer_Freigabe,
         Tuer_Sperren,
         Kneeling_2,
         Kneeling_1,
         Kindersteuerung,
         Parkbremse,
         Haltestellenansage,
         Licht_Fahrer,
         Licht_Innen,
         ASR,

         //  5V: position 25 .. 48
         Tuer_3 = 25,
         Tuer_2,
         Tuer_1,
      };

      /**
       * updates the physical state of the lamps to the logical state, if necessary
       * communicates with the shift register ICs
       */
      void update();

      /**
       * @returns the pin number to which SI is connected 
       */
      const uint8_t pinSerial() const { return _pinSerial; }

      /**
       * @returns the pin number to which SCK is connected 
       */
      const uint8_t pinSerialClock() const { return _pinSerialClock; }

      /**
       * @returns the pin number to which RCK is connected 
       */
      const uint8_t pinLatch() const { return _pinLatch; }

      /**
       * queries whether a given position is valid on the lamp driver
       * @param[in] postition the position to query
       * @returns true, if the given position is valid (note: this does not imply that a Lamp instance exists at that position)
       */
      bool isValidPosition(const uint8_t position) const { return (position < 8 * NUMBER_OF_ICS); }

      /**
       * creates an instance
       * @param[in] pinSerial the Arduino pin the lampdriver board SI is connected to
       * @param[in] pinSerialClock the Arduino pin the lampdriver board SCK is connected to
       * @param[in] pinLatch the Arduino pin the lampdriver board RCK is connected to
       */
      LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch);

      /**
       * sets the logical state of one lamp position on the lampdriver board
       * @param[in] position set the state of this lamp position (0..n)
       * @param[in] state set the lamp to this state (true = on)
       */
      void setState(const uint8_t position, const bool state);

      /**
       * queries the state of one lamp position
       * @param[in] position query the state of this lamp position
       * @param[in] queryHw queries the hardware state if set to true, the logical state otherwise
       */
      bool state(const uint8_t position, const bool queryHw = false) const;

      /**
       * switches off all lamps
       */
      void allOff();

      /**
       * initializes the instance
       * call this once in setup()
       */
      void begin();

   protected:
   private:
      const uint8_t _pinSerial;
      const uint8_t _pinSerialClock;
      const uint8_t _pinLatch;
      uint64_t _lastWriteMs = 0;
      uint8_t _lampstates[NUMBER_OF_ICS];   // logical lamp state:  one array entry (8bit) per IC; byte0, bit0 = position0; byte1, bit0 = position8 ...
      uint8_t _lampstatesHw[NUMBER_OF_ICS]; // hardware lamp state: one array entry (8bit) per IC

      /**
       * takes the lamp position on the board and derives the index into _lampstates and the bitmask from it. 
       * WARNING! does not check the plausibility of position! index will be out of boundaries (which in turn will cause a crash) when isValidPosition for that position is false!
       * @param[in] the lamp position (see warning above)
       * @param[out] index the resulting index
       * @param[out] bitmask the resulting bitmask
       */
      void calculateIndexAndBitmask(const uint8_t position, uint8_t &index, uint8_t &bitmask) const;

      /**
       * @returns true, when any lamp state change has been requested since last writeLampState (=> a shift out is necessary)
       */
      bool needsHwUpdate() const;

      // disallow creation
      LampHandler() = delete;
      LampHandler(const LampHandler &) = delete;
      LampHandler &operator=(const LampHandler &) = delete;

      /**
       * updates the hw state of the lamp pins with the logical lamp states, when necessary
       */
      void writeLampState();

      /**
       * updates the hw state of the lamp pins with the logical lamp states
       */
      void writeToHw();
   };
} // namespace BusDashboard
