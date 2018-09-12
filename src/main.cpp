/**
 * Motor Driver 2018
 */

#include "log.hpp"
#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"
#include "framework/serial_buffer.hpp"
#include "hardware/hardware.hpp"

#include <util/delay.h>

using namespace hardware;
using namespace application;

const DigitalOutputPin& pin = DigitalOutputPin(
        io_port::getB(),
        DigitalOutputPin::Bit::B5,
        io_port::PinMode::DigitalOutput);

const DigitalInputPin& switchPin = DigitalInputPin(
        io_port::getD(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

SerialBuffer serialBuffer(
        RingBuffer::Size::D128,
        usart::get1({
            38400UL,
            usart::Mode::Asynchronous,
            usart::ParityMode::Disabled,
            usart::StopBitSize::Bit1,
            usart::CharacterSize::Bit8,
            }),
        interrupt::getRx1(),
        interrupt::getTx1());

void putcharImpl(char data) {
    serialBuffer.write(data);
}

void setup() {
    log::configure(putcharImpl, log::Level::Info);
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);
    enableGlobalInterrupt();
}

void loop() {
    log_v("in");
    log_i("info");
    pin.write(switchPin.read());
    _delay_ms(1000);
    log_v("out");
}

int main() {
    setup();
    while (true) {
        loop();
    }
}

