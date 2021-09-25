#pragma once
#include "ItemNode.h"

namespace BusDashboard {

    class KomsiCommandListener;

    /**
     * handles the commands sent by KOMSI
     * 
     * to map a command to a function, edit the enum Commands
     *  e.g.: to map 'A' to Indicator, set KomsiHandler::Commands::Blinker = KomsiHandler::Codes::C01
    */
    class KomsiHandler
    {

    public:
        KomsiHandler() {}

        void begin() {}

        /**
         * this enum contains all valid command codes that may be received from KOMSI
         */
        enum Codes : uint8_t
        {
            A = 'A',
            a = 'a',
            B = 'B',
            b = 'b',
            C = 'C',
            c = 'c',
            D = 'D',
            d = 'd',
            E = 'E',
            e = 'e',
            F = 'F',
            G = 'G',
            g = 'g',
            H = 'H',
            h = 'h',
            I = 'I',
            i = 'i',
            J = 'J',
            j = 'j',
            K = 'K',
            k = 'k',
            L = 'L',
            l = 'l',
            M = 'M',
            m = 'm',
            N = 'N',
            n = 'n',
            O = 'O',
            o = 'o',
            P = 'P',
            p = 'p',
            Q = 'Q',
            q = 'q',
            R = 'R',
            r = 'r',
            S = 'S',
            s = 's',
            T = 'T',
            t = 't',
            U = 'U',
            u = 'u',
            V = 'V',
            v = 'v',
            W = 'W',
            w = 'w',
            X = 'X',
            x = 'x',
            Y = 'Y',
            y = 'y',
            Z = 'Z',
            z = 'z'
        };

        /**
         * this enum maps command codes to actual commands
         * this mapping must match the definition you made in KOMSIs .opl file!
        */
        enum Commands {
            Blinker = Codes::A
        };

        /**
        * process commands and values coming from KOMSI
        * KOMSI sends a stream like this every frame: A123B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0Y0Z0a0b0c0d0e0f0
        *   Serial.read() receives a uint16_t, but only 8 bit (actually even less) are used
        *   one command consists of a char ('A' to 'Z' and 'a' to 'z') and a corresponding command value (no documentation there, this code assumes an uint16_t range)
        */
        void processIncoming(const int value);

        /**
         * adds a listener
         * @param listener the listener that shall be added
         * @param command the listener will be informed each time this command is received
         * @return true if the listener was added successfully
        */
        bool addListener(KomsiCommandListener& listener, const uint8_t command);

    protected:

    private:
        static const uint8_t COMMAND_COUNT = 2 * 26;

        ItemNode<KomsiCommandListener> *_commandListener[COMMAND_COUNT]; // stores the listeners per command
        uint8_t _command = 0;
        uint16_t _command_value = 0;

        /**
         * calculates the index position inside the array of listeners
         * @param command calculates the index position for this command
         * @return index position for the given command (or KomsiHandler::COMMAND_COUNT + 1 if command is invalid)
        */
        static uint8_t toIndex(uint8_t command);

        /**
         * checks, whether a given index exists in the _commandListener array
         * @param index the index to check
         * @return true if the index is valid
        */
        static bool isIndexValid(uint8_t index) { return index < COMMAND_COUNT; }

        /**
         * checks whether the given char value represents a digit
         * @param value the char value to check
         * @return true, if value represents a digit
         */
        static bool isdigit(const uint8_t value) { return (value >= '0') && (value <= '9'); }

        // notify all listeners about the current command and its value
        void notifyListeners();

        // disallow creation
        KomsiHandler(const KomsiHandler &) = delete;
        KomsiHandler &operator=(const KomsiHandler &) = delete;
    };
}
