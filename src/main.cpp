/**
 * Motor Driver 2018
 */

#include "log.hpp"
#include "h_bridge.hpp"
#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"
#include "framework/serial_buffer.hpp"
#include "hardware/hardware.hpp"

#include <util/delay.h>

using namespace hardware;
using namespace application;

extern "C" void __cxa_pure_virtual() {
    log_e("fatal error occured");
    while (1);
}

DigitalOutputPin pin(
        io_port::getB(),
        DigitalOutputPin::Bit::B5,
        io_port::PinMode::DigitalOutput);

// pins for h-bridge 0
DigitalOutputPin pin_b0l1(
        io_port::getB(),
        DigitalOutputPin::Bit::B0,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b0h0(
        io_port::getB(),
        DigitalOutputPin::Bit::B1,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b0h1(
        io_port::getB(),
        DigitalOutputPin::Bit::B2,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b0l0(
        io_port::getD(),
        DigitalOutputPin::Bit::B7,
        io_port::PinMode::DigitalOutput);

H_Bridge bridge0(
        pwm::get1A({}),
        pwm::get1B({}),
        pwm::OutputMode::NonInverting,
        pwm::OutputMode::Disabled,
        pin_b0l0, pin_b0h0, pin_b0l1, pin_b0h1,
        interrupt::getOC1A(),
        interrupt::getOC1B(),
        pwm::get3A({}),
        interrupt::getOC3A(),
        interrupt::getTimer1Ovf());

// pins for h-bridge 1
DigitalOutputPin pin_b1l0(
        io_port::getD(),
        DigitalOutputPin::Bit::B0,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b1h0(
        io_port::getD(),
        DigitalOutputPin::Bit::B1,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b1h1(
        io_port::getD(),
        DigitalOutputPin::Bit::B2,
        io_port::PinMode::DigitalOutput);
DigitalOutputPin pin_b1l1(
        io_port::getD(),
        DigitalOutputPin::Bit::B3,
        io_port::PinMode::DigitalOutput);

H_Bridge bridge1(
        pwm::get4A({}),
        pwm::get4B({}),
        pwm::OutputMode::NonInverting,
        pwm::OutputMode::Disabled,
        pin_b1l0, pin_b1h0, pin_b1l1, pin_b1h1,
        interrupt::getOC4A(),
        interrupt::getOC4B(),
        pwm::get3B({}),
        interrupt::getOC3B(),
        interrupt::getTimer4Ovf());

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

Timer& timer1 = timer::get1({
        timer::WaveformGenerationMode::FastPWM_10bit,
        timer::InputCaptureNoiseCanceler::Disabled,
        timer::InputCaptureEdgeSelect::RisingEdge,
        timer::Clock::Div8,
        });

Timer& timer3 = timer::get3({
        timer::WaveformGenerationMode::FastPWM_10bit,
        timer::InputCaptureNoiseCanceler::Disabled,
        timer::InputCaptureEdgeSelect::RisingEdge,
        timer::Clock::Div8,
        });

Timer& timer4 = timer::get4({
        timer::WaveformGenerationMode::FastPWM_10bit,
        timer::InputCaptureNoiseCanceler::Disabled,
        timer::InputCaptureEdgeSelect::RisingEdge,
        timer::Clock::Div8,
        });

void setup() {
    // system clock freq = 8MHz div 1
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);

    // configure log module
    auto putCharImpl = [](char data) { serialBuffer.write(data); };
    log::configure(+putCharImpl, log::Level::Info);

    timer1.setCount(0);
    timer3.setCount(timer1.getCount());
    timer4.setCount(timer1.getCount());
    timer1.start();
    timer3.start();
    timer4.start();

    bridge0.changeDirection(H_Bridge::Direction::Backward);
    bridge1.changeDirection(H_Bridge::Direction::Backward);

    bridge0.setDutyRatio(1.00);
    bridge1.setDutyRatio(0.00);

    // must be called here
    enableGlobalInterrupt();

}

void loop() {
    static float dutyRatio = 1.0;
    log_v("in");
    log_i("ratio = %d", static_cast<int>(dutyRatio * 100));
    bridge1.setDutyRatio(dutyRatio);
    dutyRatio -= 0.1;
    if (dutyRatio < 0.0) {
        dutyRatio = 1.0;
    }
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

