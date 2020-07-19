#pragma once
#include <Arduino.h>
//#include <stdint.h>

namespace BusDashboard {

	class LampHandler {
	
	public:

        static const uint8_t NUMBER_OF_ICS = 6;
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

        LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch);

        void setState(const uint8_t position, const bool state);

        bool state(const uint8_t position, const bool queryHw = false) const;

	protected:


	private:
        const uint8_t _pinSerial;
        const uint8_t _pinSerialClock;
        const uint8_t _pinLatch;
        uint8_t _lampstates[NUMBER_OF_ICS];   // logical lamp state:  vector of bitarrays, one 8bit-Array per IC
        uint8_t _lampstatesHw[NUMBER_OF_ICS]; // hardware lamp state: vector of bitarrays, one 8bit-Array per IC

        // disallow creation
		LampHandler() = delete;
        LampHandler(const LampHandler&) = delete;
        LampHandler& operator=(const LampHandler&) = delete;

 		/**
          * updates the hw state of the lamp pins with the logical lamp states
          */
        void writeLampState();
	};
}
