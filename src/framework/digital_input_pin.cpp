#include "digital_input_pin.hpp"

DigitalInputPin::DigitalInputPin(const IOPort& port, Bit bit, PinMode mode)
    : port(port), bit(bit) {
    port.setPinModes(bit, mode);
}

bool DigitalInputPin::read() {
    return (port.read() & bit) == 0 ? false : true;
}
