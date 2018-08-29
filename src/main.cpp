/**
 * Motor Driver 2018
 */

#include <avr/io.h>
#include <util/delay.h>

#include "hardware/hardware.hpp"

using namespace hardware;

const IOPort& portB = getPortB();

void setup() {
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);
    portB.setPinModes(_BV(5), io_port::PinMode::DigitalOutput);
}

void loop() {
    _delay_ms(1000);
    portB.write(_BV(5), 0xFF);
    _delay_ms(1000);
    portB.write(_BV(5), 0x00);
}

int main() {
    setup();
    while (true) {
        loop();
    }
}
