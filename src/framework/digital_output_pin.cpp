#include "digital_output_pin.hpp"

DigitalOutputPin::DigitalOutputPin(const IOPort& port, Bit bit, PinMode mode) : port(port), bit(bit) {
    port.setPinModes(bit, mode);
}

void DigitalOutputPin::write(bool level) const {
    const uint8_t output = level == true ? 0xff : 0x00;
    port.write(bit, output);
}

void DigitalOutputPin::toggle() const {
    port.toggle(bit);
}
