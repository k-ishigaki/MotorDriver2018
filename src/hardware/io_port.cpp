#include "io_port.hpp"
#include "hardware.hpp"
#include <avr/io.h>

using namespace hardware::io_port;

#define JOIN(x, y) JOINX(x, y)
#define JOINX(x, y) x##y

#define CLASS_NAME(x) JOIN(Port, x)

#define DDR(x) JOIN(DDR, x)
#define PORT(x) JOIN(PORT, x)
#define PIN(x) JOIN(PIN, x)

#if !defined(PORTB_DECRARED)

#define PORTB_DECRARED
#define X B

#elif !defined(PORTC_DECRARED)

#define PORTC_DECRARED
#define X C

#elif !defined(PORTD_DECRARED)

#define PORTD_DECRARED
#define X D

#elif !defined(PORTE_DECRARED)

#define PORTE_DECRARED
#define X E

#define EXIT_LOOP
#endif

class CLASS_NAME(X) : IOPort {
    public:
        void setPinModes(uint8_t positions, PinMode mode) const {
            switch (mode) {
                case PinMode::DigitalOutput:
                    DDR(X) |= positions;
                    break;
                case PinMode::DigitalInput:
                    DDR(X) &= ~positions;
                    PORT(X) &= ~positions;
                    break;
                case PinMode::DigitalInputWithPullUp:
                    DDR(X) &= ~positions;
                    PORT(X) |= positions;
                    break;
            }
        }

        uint8_t read() const {
            return PIN(X);
        }

        void write(uint8_t positions, uint8_t outputs) const {
            PORT(X) = (~positions & PORT(X)) | (positions & outputs);
        }

        void toggle(uint8_t positions) const {
            // Writing a '1' to PINxn toggles the value of PORTxn,
            // independent on the value of DDRxn
            PIN(X) |= positions;
        }
};

const IOPort& hardware::io_port::JOIN(get, CLASS_NAME(X))() {
    static CLASS_NAME(X) instance;
    return (IOPort&)instance;
}

#undef X

#if !defined(EXIT_LOOP)
#include "io_port.cpp"
#endif
