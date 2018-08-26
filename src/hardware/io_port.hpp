#ifndef IO_PORT_H
#define IO_PORT_H

#include <stdint.h>

namespace hardware {
    enum IOPort_PinMode : uint8_t;
}

/**
 * Defines IO Port interfaces.
 *
 * A function of ADC is not included because ADC module is not a part of
 * IO port module.
 *
 * usage: write to output latch
 * {@code
 * IOPort* port = Hardware::PortA; // get instance of Port A
 * // set all pins to digital output
 * port->setPinModes(0xFF, IOPort_PinMode.DIGITAL_OUTPUT);
 * port->write(0x55);     // pin 0, 2, 4, 6: output high
 *                        // pin 1, 3, 5, 7: output low
 * 
 * usage: read port status
 * {@code
 * IOPort* port = Hardware::PortA;
 * // set all pins to digital input
 * port->setPinModes(0xFF, IOPort_PinMode.DIGITAL_INPUT);
 * uint8_t result = port->read(); // read port and store
 * }
 */
class IOPort {
    public:
        /**
         * Apply pin mode to selected pins.
         *
         * You can select pins at first argument, and pin mode at second
         * argument.
         * The pin mode is defined at Hardware.h.
         * Please check data sheet not to select unsupported pins.
         *
         * For example:
         * {@code
         * // set pin 0 and pin 4 to digital input
         * port->setPinModes(0b00010001, IOPort_PinMode.DIGITAL_INPUT);
         * }
         *
         * @param pin positions
         * @param pin mode
         */
        virtual void setPinModes(uint8_t, hardware::IOPort_PinMode) const = 0;

        /**
         * Read levels on the digital input pins.
         *
         * If a pin is not digital input, the return value is not specified.
         *
         * @return 1 if high level , else 0 for respective pins
         */
        virtual uint8_t read(void) const = 0;

        /**
         * Write outputs to digital output pins.
         *
         * If a pin is not digital output or not selected by first argument,
         * its state is unchanged.
         *
         * @param pin positions
         * @param a output levels for respective pins<br>
         * 1 = output high<br>
         * 0 = output low
         */
        virtual void write(uint8_t, uint8_t) const = 0;

        /**
         * Toggle outputs of digital output pins.
         *
         * If a pin is not digital output of not selected by first arguments,
         * its status is unchanged.
         *
         * @param pin positions
         */
        virtual void toggle(uint8_t) const = 0;
};

#endif
