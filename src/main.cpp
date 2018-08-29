/**
 * Motor Driver 2018
 */

#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"
#include "hardware/hardware.hpp"

#include <avr/io.h>
#include <util/delay.h>

using namespace hardware;

const DigitalOutputPin& pin = DigitalOutputPin(
        getPortB(),
        DigitalOutputPin::Bit::B5,
        io_port::PinMode::DigitalOutput);

const DigitalInputPin& switchPin = DigitalInputPin(
        getPortD(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

void setup() {
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);
}

void loop() {
    pin.write(switchPin.read());
}

int main() {
    setup();
    while (true) {
        loop();
    }
}
