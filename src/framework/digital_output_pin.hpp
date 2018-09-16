#ifndef DIGITAL_OUTPUT_PIN_H
#define DIGITAL_OUTPUT_PIN_H

#include "../hardware/io_port.hpp"

class DigitalOutputPin {
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
         * Digital output pin constructor.
         *
         * The 3rd param must be correctly supplied to work correctly.
         *
         * @param IOPort interface
         * @param position for port
         * @param digital output pin mode
         */
        DigitalOutputPin(const IOPort&, Bit, PinMode);

        /**
         * Write output to pin.
         *
         * @param true for HIGH level ouput, false for LOW
         */
        void write(bool) const;

        /**
         * Toggle output of pin.
         */
        void toggle() const;

    private:
        const IOPort& port;
        const Bit bit;
};

#endif
