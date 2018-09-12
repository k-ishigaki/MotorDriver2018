#include "io_port.hpp"
#include "../device/io_port.hpp"
#include "hardware.hpp"

using namespace device::io_port;
using namespace hardware::io_port;

template<uint8_t offset> class IOPort_ : IOPort {
    using regs = io_port_t<offset>;
    public:
        static IOPort& getInstance() {
            static IOPort_<offset> instance;
            return instance;
        }

        void setPinModes(uint8_t positions, PinMode mode) const override {
            switch (mode) {
                case PinMode::DigitalOutput:
                    regs::DDR |= positions;
                    break;
                case PinMode::DigitalInput:
                    regs::DDR &= ~positions;
                    regs::PORT &= ~positions;
                    break;
                case PinMode::DigitalInputWithPullUp:
                    regs::DDR &= ~positions;
                    regs::PORT |= positions;
                    break;
            }
        }

        uint8_t read() const override {
            return regs::PIN;
        }

        void write(uint8_t positions, uint8_t outputs) const override {
            regs::PORT = (~positions & regs::PORT) | (positions & outputs);
        }

        void toggle(uint8_t positions) const override {
            // Writing a '1' to PINxn toggles the value of PORTxn,
            // independent on the value of DDRxn
            regs::PIN |= positions;
        }
};

const IOPort& hardware::io_port::getB() {
    return IOPort_<0x00>::getInstance();
}

const IOPort& hardware::io_port::getC() {
    return IOPort_<0x03>::getInstance();
}

const IOPort& hardware::io_port::getD() {
    return IOPort_<0x06>::getInstance();
}

const IOPort& hardware::io_port::getE() {
    return IOPort_<0x09>::getInstance();
}
