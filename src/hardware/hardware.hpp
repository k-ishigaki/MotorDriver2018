#ifndef HARDWARE_H
#define HARDWARE_H

#include "ad_converter.hpp"
#include "interrupt.hpp"
#include "io_port.hpp"
#include "usart.hpp"

namespace hardware {
    namespace SystemClockPrescaler {
        /**
         * Division Factor for system clock prescaler.
         */
        enum class DivisionFactor {
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

    namespace interrupt {
        const Interrupt& getTx0();
        const Interrupt& getTx1();
        const Interrupt& getRx0();
        const Interrupt& getRx1();
    }

    namespace io_port {
        enum class PinMode : uint8_t {
            DigitalInput,
            DigitalOutput,
            DigitalInputWithPullUp,
        };

        const IOPort& getB();
        const IOPort& getC();
        const IOPort& getD();
        const IOPort& getE();
    }

    namespace usart {
        enum class Error : error_flag_t {

        };

        enum class Mode : uint8_t {
            Asynchronous = 0b00,
            Synchronous = 0b01,
        };

        enum class ParityMode : uint8_t {
            Disabled = 0b00,
            EvenParity = 0b10,
            OddParity = 0b11,
        };

        enum class StopBitSize : uint8_t {
            Bit1 = 0b0,
            Bit2 = 0b1,
        };

        enum class CharacterSize : uint8_t {
            Bit5 = 0b000,
            Bit6 = 0b001,
            Bit7 = 0b010,
            Bit8 = 0b011,
            Bit9 = 0b111,
        };

        struct Config {
            baudrate_size_t baudrate;
            Mode mode;
            ParityMode parityMode;
            StopBitSize stopBitSize;
            CharacterSize characterSize;
        };

        const USART& get0(const Config&);
        const USART& get1(const Config&);
    }
    ADConverter* getADConverter();
}

#endif
