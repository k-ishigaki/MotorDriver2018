/**
 * Motor Driver 2018
 */

#include "encoder.hpp"
#include "log.hpp"
#include "h_bridge.hpp"
#include "odmetry.hpp"
#include "periodic_tiemr.hpp"
#include "speed_controller.hpp"
#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"
#include "framework/i2c_buffer.hpp"
#include "framework/serial_buffer.hpp"
#include "hardware/hardware.hpp"

#include <util/delay.h>

using namespace hardware;
using namespace application;

extern "C" void __cxa_pure_virtual() {
    log_e("fatal error occured");
    while (1);
}

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

DigitalInputPin switchPin(
        io_port::getD(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

SerialBuffer serialBuffer(
        RingBuffer::Size::D128,
        usart::get1({
            .baudrate = 115200UL,
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

// encoder pins
DigitalOutputPin led_encoder0(
        io_port::getB(),
        DigitalOutputPin::Bit::B5,
        io_port::PinMode::DigitalOutput);

DigitalOutputPin led_encoder1(
        io_port::getC(),
        DigitalOutputPin::Bit::B1,
        io_port::PinMode::DigitalOutput);

Timer& timer2 = timer2::get2({
        timer2::WaveformGenerationMode::Normal,
        timer2::Clock::Div256,
        });

PeriodicTimer periodicTimer(interrupt::getTimer2Ovf());

Encoder encoder0(interrupt::getPCI3(), led_encoder0);
Encoder encoder1(interrupt::getPCI1(), led_encoder1);

I2CSlave& i2cSlave = i2c::getTwi0({
        .slaveAddress = 0x60,
        .generalCallReception = i2c::GeneralCallReception::Disable
        });

I2CBuffer i2cBuffer(i2cSlave, interrupt::getTwi0(), 0x08, 6);

Odmetry odmetry(encoder0, encoder1, 180.0);

SpeedController speedController(encoder0, encoder1, bridge0, bridge1);

struct odmetryData_t {
    int16_t odmetry_x = 0x5599;
    int16_t odmetry_y = 0x1122;
    int16_t odmetry_theta = 0x1144;
} odmetryData;

uint8_t odmetryResetFlag = 0;

struct targetSpeedData_t {
    int16_t left = 0;
    int16_t right = 0;
} targetSpeedData;

void setup() {
    // system clock freq = 8MHz div 1
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);

    // configure log module
    auto putCharImpl = [](char data) { serialBuffer.write(data); };
    log::configure(+putCharImpl, log::Level::Vervose);

    log_e("error log test");

    timer1.setCount(0);
    timer3.setCount(timer1.getCount());
    timer4.setCount(timer1.getCount());
    timer1.start();
    timer3.start();
    timer4.start();
    ::timer2.start();

    bridge0.changeDirection(H_Bridge::Direction::Backward);
    bridge1.changeDirection(H_Bridge::Direction::Backward);

    bridge0.setDutyRatio(0.50);
    bridge1.setDutyRatio(0.50);

    i2cBuffer.registerData(0x01, reinterpret_cast<uint8_t*>(&odmetryData), sizeof(odmetryData_t));
    i2cBuffer.registerData(0x02, reinterpret_cast<uint8_t*>(&odmetryResetFlag), sizeof(uint8_t));
    i2cBuffer.registerData(0x03, reinterpret_cast<uint8_t*>(&targetSpeedData), sizeof(targetSpeedData_t));

    PinChangeInterrupt& pci1 = hardware::pin_change_interrupt::getPCI1();
    pci1.enable(1 << 0);
    PinChangeInterrupt& pci3 = hardware::pin_change_interrupt::getPCI3();
    pci3.enable(1 << 3);

    // must be called here
    enableGlobalInterrupt();
}

void updateOdmetryData() {
    auto f = []() {
        Vector machineVector = odmetry.getMachineVector();
        odmetryData.odmetry_x = machineVector.getX();
        odmetryData.odmetry_y = machineVector.getY();
        odmetryData.odmetry_theta = 1000 * machineVector.getTheta();
    };
    Vector machineVector = odmetry.getMachineVector();
    //log_v("x = %d, y = %d, theta = %d", 
    //        static_cast<int>(machineVector.getX()),
    //        static_cast<int>(machineVector.getY()),
    //        static_cast<int>(1000 * machineVector.getTheta()));
    hardware::noInterrupt(+f);
}

void resetOdmetryData() {
    auto f = []() { return odmetryResetFlag; };
    auto g = []() { odmetryResetFlag = 0; };
    if (hardware::noInterruptWithReturn(+f) != 0) {
        hardware::noInterrupt(+g);
        Vector vector(Point(0, 0), 0);
        odmetry.modifyDifference(vector);
    }
}

void updateTargetSpeed() {
    auto f = []() {
        speedController.setTarget(targetSpeedData.left, 0);
    };
    hardware::noInterrupt(+f);
}

void loop() {
    if (periodicTimer.wasInterruptOccured() == true) {
        encoder0.load();
        encoder1.load();
        resetOdmetryData();
        odmetry.update();
        updateOdmetryData();
        updateTargetSpeed();
        speedController.determineOutput();
    }
}

int main() {
    setup();
    while (true) {
        loop();
    }
}

