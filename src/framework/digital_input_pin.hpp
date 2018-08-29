#ifndef DIGITAL_INPUT_PIN_H
#define DIGITAL_INPUT_PIN_H

#include "../hardware/io_port.hpp"

class DigitalInputPin {
    public:
        /**
         * Bit position definition.
         */
        enum Bit {
            B0 = 0b00000001,
            B1 = 0b00000010,
            B2 = 0b00000100,
            B3 = 0b00001000,
            B4 = 0b00010000,
            B5 = 0b00100000,
            B6 = 0b01000000,
            B7 = 0b10000000,
        };

        /**
         * Digital Pin Constructor.
         *
         * 3rd param are must be correctly supplied to work correctly.
         *
         * @param port IOPort interface
         * @param bit position for port
         * @param mode digital input pin mode
         */
        DigitalInputPin(const IOPort&, Bit, PinMode);

        /**
         * Read input bit state.
         *
         * Prease ensure a pin is digital input mode before call this method.
         *
         * @return true if input is HIGH level, false if otherwise
         */
        bool read(void);

    private:
        const IOPort& port;
        const Bit bit;
};

#endif
