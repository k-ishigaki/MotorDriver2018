#ifndef HARDWARE_H
#define HARDWARE_H

#include "hardware_implementation.hpp"

#include "ad_converter.hpp"
#include "i2c.hpp"
#include "interrupt.hpp"
#include "io_port.hpp"
#include "pin_change_interrupt.hpp"
#include "pwm.hpp"
#include "usart.hpp"
#include "timer.hpp"

namespace hardware {
    namespace system_clock_prescaler {
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

    namespace i2c {
        enum class GeneralCallReception {
            Disable = 0,
            Enable = 1,
        };

        struct Config {
            uint8_t slaveAddress;
            GeneralCallReception generalCallReception;
        };

        I2CSlave& getTwi0(const Config&);
        I2CSlave& getTwi1(const Config&);
    }

    namespace interrupt {
        Interrupt& getTwi0();
        Interrupt& getTwi1();
        Interrupt& getTx0();
        Interrupt& getTx1();
        Interrupt& getRx0();
        Interrupt& getRx1();
        Interrupt& getTimer1Ovf();
        Interrupt& getOC1A();
        Interrupt& getOC1B();
        Interrupt& getTimer3Ovf();
        Interrupt& getOC3A();
        Interrupt& getOC3B();
        Interrupt& getTimer4Ovf();
        Interrupt& getOC4A();
        Interrupt& getOC4B();
        Interrupt& getTimer2Ovf();
        Interrupt& getOC2A();
        Interrupt& getOC2B();
        Interrupt& getPCI0();
        Interrupt& getPCI1();
        Interrupt& getPCI2();
        Interrupt& getPCI3();
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

    namespace pin_change_interrupt {
        PinChangeInterrupt& getPCI0();
        PinChangeInterrupt& getPCI1();
        PinChangeInterrupt& getPCI2();
        PinChangeInterrupt& getPCI3();
    }

    namespace pwm {
        enum class OutputMode : uint8_t {
            Disabled = 0b00,
            Toggle = 0b01,
            NonInverting = 0b10,
            Inverting = 0b11,
        };

        struct Config {
        };

        PWM& get1A(const Config&);
        PWM& get1B(const Config&);
        PWM& get3A(const Config&);
        PWM& get3B(const Config&);
        PWM& get4A(const Config&);
        PWM& get4B(const Config&);
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

    namespace timer {

        enum class WaveformGenerationMode {
            Normal = 0b0000,
            PWM_PhaseCorrect_8bit = 0b0001,
            PWM_PhaseCorrect_9bit = 0b0010,
            PWM_PhaseCorrect_10bit = 0b0011,
            CTC_OCRA = 0b0100,
            FastPWM_8bit = 0b0101,
            FastPWM_9bit = 0b0110,
            FastPWM_10bit = 0b0111,
            PWM_PhaseAndFrequencyCorrect_ICR = 0b1000,
            PWM_PhaseAndFrequencyCorrect_OCRA = 0b1001,
            PWM_PhaseCorrect_ICR = 0b1010,
            PWM_PhaseCorrect_OCRA = 0b1011,
            CTC_ICR = 0b1100,
            //Reserved = 0b1101,
            FastPWM_ICR = 0b1110,
            FastPWM_OCRA = 0b1111,
        };

        enum class InputCaptureNoiseCanceler {
            Disabled = 0b0,
            Enabled = 0b1,
        };

        enum class InputCaptureEdgeSelect {
            FallingEdge = 0b0,
            RisingEdge = 0b1,
        };

        enum class Clock {
            NoClockSource = 0b000,
            Div1 = 0b001,
            Div8 = 0b010,
            Div64 = 0b011,
            Div256 = 0b100,
            Div1024 = 0b101,
            External_FallingEdge = 0b110,
            External_RisingEdge = 0b111,
        };

        struct Config {
            WaveformGenerationMode waveformGenerationMode;
            InputCaptureNoiseCanceler inputCaptureNoiseCanceler;
            InputCaptureEdgeSelect inputCaptureEdgeSelect;
            Clock clock;
        };

        Timer& get1(const Config&);
        Timer& get3(const Config&);
        Timer& get4(const Config&);
    }

    namespace timer2 {
        enum class WaveformGenerationMode {
            Normal = 0b000,
            PWM_PhaseCorrect = 0b001,
            CTC_OCRA = 0b010,
            FastPWM = 0b011,
            //Reserved = 0b100,
            PWM_PhaseCorrect_OCRA = 0b101,
            //Reserved = 0b110,
            FastPWM_OCRA = 0b111,
        };

        enum class Clock {
            NoClockSource = 0b000,
            Div1 = 0b001,
            Div8 = 0b010,
            Div32 = 0b011,
            Div64 = 0b100,
            Div128 = 0b101,
            Div256 = 0b110,
            Div1024 = 0b111,
        };

        struct Config {
            WaveformGenerationMode waveformGenerationMode;
            Clock clock;
        };

        Timer& get2(const Config&);
    }

    ADConverter* getADConverter();
}

#endif
