#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <vector>

namespace BusDashboard {

    class LampHandler;

    class Lamp {
        private:
            LampHandler& _parent;
            const uint8_t _position; // connected to which logical pin number (0 .. numberOfChips * 8 - 1)

            // disallow creation
            Lamp() = delete;
            Lamp(const Lamp&) = delete;
            Lamp& operator=(const Lamp&) = delete;

        protected:
            Lamp(LampHandler& parent, const uint8_t position):_parent(parent),_position(position) {}

        public:
            /**
             * switches the lamp on
             */
            void on() { setState(true); };

            /**
             * switches the lamp off
             */
            void off() { setState(false); };

            /**
             * queries the logical lamp state
             * @returns the current logical lamp state (the state the hardware will be switched to on the next update)
             */
            bool state() const;

            /**
             * queries the hardware lamp state
             * @returns the current hardware lamp state
             */
            bool hwstate() const;

            /**
             * sets the logical lamp state
             * @param[in] newState
             */
            void setState(const bool state);

            /**
             * @returns the position of this Lamp on the lamp driver
             */
            uint8_t position() const { return _position; }

        friend class LampHandler;
    };

	class LampHandler {
	
	public:
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
		 * @returns the number of 74HC595 chips the lamp driver consists of 
		 */
        const uint8_t icCount() const { return _numberOfIcs; }

 		/**
          * queries the Lamp instanve at a specific pin pair positions
          * @param[in] postition the position to query
          * @returns the Lamp instance at that position (may be nullptr)
          */
        Lamp * getLampAtPosition(const uint8_t position) { return isValidPosition(position) ? _lamps[position] : nullptr; }

 		/**
          * queries whether a given position is valid on the lamp driver
          * @param[in] postition the position to query
          * @returns true, if the given position is valid (note: this does not imply that a Lamp instance exists at that position)
          */
        bool isValidPosition(const uint8_t position) const { return (position < 8 * _numberOfIcs); }

        LampHandler(const uint8_t pinSerial, const uint8_t pinSerialClock, const uint8_t pinLatch, const uint8_t numberOfICs);


	protected:
        void setState(const uint8_t position, const bool state);
        bool state(const uint8_t position, const bool queryHw = false) const;


	private:
        const uint8_t _pinSerial;
        const uint8_t _pinSerialClock;
        const uint8_t _pinLatch;
        const uint8_t _numberOfIcs;
        std::vector<Lamp*> _lamps;          // Lamp instances
        std::vector<uint8_t> _lampstates;   // logical lamp state:  vector of bitarrays, one 8bit-Array per IC
        std::vector<uint8_t> _lampstatesHw; // hardware lamp state: vector of bitarrays, one 8bit-Array per IC

        // disallow creation
		LampHandler() = delete;
        LampHandler(const LampHandler&) = delete;
        LampHandler& operator=(const LampHandler&) = delete;

 		/**
          * updates the hw state of the lamp pins with the logical lamp states
          */
        void writeLampState();

        friend class Lamp;
	};
}
