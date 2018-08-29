#ifndef HARDWARE_H
#define HARDWARE_H

#include "ad_converter.hpp"
#include "io_port.hpp"

namespace hardware {
    namespace SystemClockPrescaler {
        /**
         * Division Factor for system clock prescaler.
         */
        enum DivisionFactor {
            Num1 = 0b0000,
            Num2 = 0b0001,
            Num4 = 0b0010,
            Num8 = 0b0011,
            Num16 = 0b0100,
            Num32 = 0b0101,
            Num64 = 0b0110,
            Num128 = 0b0111,
            Num256 = 0b1000,
        };
        /**
         * Configure a clock division factor for System Oscillator.
         *
         * @param one of {@link Hardware#SystemClockPrescaler#DivisionFactor}
         */
        void configure(DivisionFactor);
    }

    namespace io_port {
        enum PinMode : uint8_t {
            DigitalInput,
            DigitalOutput,
            DigitalInputWithPullUp,
        };

        const IOPort& getPortB();
        const IOPort& getPortC();
        const IOPort& getPortD();
        const IOPort& getPortE();
    }
    ADConverter* getADConverter();
}

#endif
