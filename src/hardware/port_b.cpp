#include "io_port.hpp"
#include "hardware.hpp"

#include <avr/io.h>

using namespace hardware;

class PortB : IOPort {
    public:
        void setPinModes(uint8_t positions, IOPort_PinMode mode) const {
            switch (mode) {
                case IOPort_PinMode::DigitalOutput:
                    DDRB |= positions;
                    break;
                case IOPort_PinMode::DigitalInput:
                    DDRB &= ~positions;
                    PORTB &= ~positions;
                    break;
                case IOPort_PinMode::DigitalInputWithPullUp:
                    DDRB &= ~positions;
                    PORTB |= positions;
                    break;
            }
        }

        uint8_t read() const {
            return PINB;
        }

        void write(uint8_t positions, uint8_t outputs) const {
            PORTB |= (positions & outputs);
            PORTB &= (~positions | outputs);
        }

        void toggle(uint8_t positions) const {
            // Writing a '1' to PINxn toggles the value of PORTxn,
            // independent on the value of DDRxn
            PINB |= positions;
        }

        static IOPort& getInstance() {
            static PortB instance;
            return instance;
        }
};

const IOPort& hardware::getPortB() {
    return PortB::getInstance();
}
