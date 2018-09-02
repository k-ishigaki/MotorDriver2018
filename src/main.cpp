/**
 * Motor DrivegetDgetDr 2018
 */

#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"
#include "hardware/hardware.hpp"

#include <util/delay.h>

using namespace hardware;

const DigitalOutputPin& pin = DigitalOutputPin(
        io_port::getB(),
        DigitalOutputPin::Bit::B5,
        io_port::PinMode::DigitalOutput);

const DigitalInputPin& switchPin = DigitalInputPin(
        io_port::getD(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

const USART& serial = usart::get1({
        38400UL,
        usart::Mode::Asynchronous,
        usart::ParityMode::Disabled,
        usart::StopBitSize::Bit1,
        usart::CharacterSize::Bit8,
        });

void setup() {
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);
}

void loop() {
    pin.write(switchPin.read());
    _delay_ms(1000);
    serial.write('A');
}

int main() {
    setup();
    while (true) {
        loop();
    }
}
