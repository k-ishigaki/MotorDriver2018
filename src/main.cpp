/**
 * Motor Driver 2018
 */

#include "config.hpp"
#include "encoder.hpp"
#include "locus.hpp"
#include "log.hpp"
#include "h_bridge.hpp"
#include "odmetry.hpp"
#include "periodic_tiemr.hpp"
#include "speed_controller.hpp"
#include "vehicle_controller.hpp"
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

H_Bridge rightFullBridge(
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

H_Bridge leftFullBridge(
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

DigitalInputPin sdaPullupPin(
        io_port::getC(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

DigitalInputPin sclPullupPin(
        io_port::getC(),
        DigitalInputPin::Bit::B5,
        io_port::PinMode::DigitalInputWithPullUp);

DigitalInputPin switchPin(
        io_port::getD(),
        DigitalInputPin::Bit::B4,
        io_port::PinMode::DigitalInputWithPullUp);

DigitalOutputPin switchLedPin(
        io_port::getC(),
        DigitalOutputPin::Bit::B3,
        io_port::PinMode::DigitalOutput);

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

DigitalInputPin encoder0Input(
        io_port::getE(),
        DigitalInputPin::Bit::B3,
        io_port::PinMode::DigitalInputWithPullUp);

DigitalOutputPin led_encoder1(
        io_port::getC(),
        DigitalOutputPin::Bit::B1,
        io_port::PinMode::DigitalOutput);

DigitalInputPin encoder1Input(
        io_port::getC(),
        DigitalInputPin::Bit::B0,
        io_port::PinMode::DigitalInputWithPullUp);

Timer& timer2 = timer2::get2({
        timer2::WaveformGenerationMode::Normal,
        timer2::Clock::Div256,
        });

PeriodicTimer periodicTimer(interrupt::getTimer2Ovf());

Encoder rightEncoder(interrupt::getPCI3(), encoder0Input, led_encoder0);
Encoder leftEncoder(interrupt::getPCI1(), encoder1Input, led_encoder1);

I2CSlave& i2cSlave = i2c::getTwi0({
        .slaveAddress = 0x60,
        .generalCallReception = i2c::GeneralCallReception::Disable
        });

InterruptHandler* i2cHandler = nullptr;

class SoftwareI2cInterrupt : public Interrupt {
    public:
        void registerHandler(InterruptHandler* handler) override {
            i2cHandler = handler;
        }
        bool isEnable() override { return true; }
        void enable() override {}
        void disable() override {}
};

SoftwareI2cInterrupt softwareI2cInterrupt;

I2CBuffer i2cBuffer(i2cSlave, softwareI2cInterrupt, 0x08, 6);

Odmetry odmetry(leftEncoder, rightEncoder);

SpeedController speedController(leftEncoder, rightEncoder, leftFullBridge, rightFullBridge);

VehicleController vehicleController(speedController);

// I2C data
struct odmetryData_t {
    int16_t odmetry_x = 0;
    int16_t odmetry_y = 0;
    int16_t odmetry_theta = 0;
} odmetryData;

uint8_t odmetryResetFlag = 0;

struct targetSpeedData_t {
    int16_t left = 0;
    int16_t right = 0;
} targetSpeedData;

struct targetStrightData_t {
    uint16_t length = 0;
    uint16_t speed = 0;
} targetStrightData;

struct targetRotateData_t {
    int16_t theta = 0;
    uint16_t speed = 0;
} targetRotateData;

uint16_t targetLocusSpeed = 0;

bool isInOperation = false;

LocusElement* locusElement = nullptr;

void setup() {
    // system clock freq = 8MHz div 1
    SystemClockPrescaler::configure(SystemClockPrescaler::DivisionFactor::Num1);

    // configure log module
    auto putCharImpl = [](char data) { serialBuffer.write(data); };
    //auto putCharImpl = [](char data) {};
    log::configure(+putCharImpl, log::Level::Vervose);

    log_e("error log test");

    timer1.setCount(0);
    timer3.setCount(timer1.getCount());
    timer4.setCount(timer1.getCount());
    timer1.start();
    timer3.start();
    timer4.start();
    ::timer2.start();

    rightFullBridge.changeDirection(H_Bridge::Direction::Backward);
    leftFullBridge.changeDirection(H_Bridge::Direction::Backward);

    rightFullBridge.setDutyRatio(0.50);
    leftFullBridge.setDutyRatio(0.50);

    i2cBuffer.registerData(0x01, reinterpret_cast<uint8_t*>(&odmetryData), sizeof(odmetryData_t));
    i2cBuffer.registerData(0x02, reinterpret_cast<uint8_t*>(&odmetryResetFlag), sizeof(uint8_t));
    i2cBuffer.registerData(0x03, reinterpret_cast<uint8_t*>(&targetSpeedData), sizeof(targetSpeedData_t));
    i2cBuffer.registerData(0x04, reinterpret_cast<uint8_t*>(&targetStrightData), sizeof(targetStrightData_t));
    i2cBuffer.registerData(0x05, reinterpret_cast<uint8_t*>(&targetRotateData), sizeof(targetRotateData_t));
    i2cBuffer.registerData(0x06, reinterpret_cast<uint8_t*>(&isInOperation), sizeof(bool));

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
        auto leftSpeed = targetSpeedData.left;
        auto rightSpeed = targetSpeedData.right;
        leftSpeed = leftSpeed > 500 ? 500 : leftSpeed;
        leftSpeed = leftSpeed < 0 ? 0 : leftSpeed;
        rightSpeed = rightSpeed > 500 ? 500 : rightSpeed;
        rightSpeed = rightSpeed < 0 ? 0 : rightSpeed;
        auto centerSpeed = (leftSpeed + rightSpeed) / 2.0;
        float curvature = 0;
        if (centerSpeed > 0) {
            curvature = 2.0 * (rightSpeed - leftSpeed) / config::WheelTread / (leftSpeed + rightSpeed);
        } else {
            curvature = 0;
        }
        vehicleController.setTarget(curvature, 1000, centerSpeed, centerSpeed);
    };
    hardware::noInterrupt(+f);
}

void updateTargetStright() {
    auto f = []() {
        auto length = targetStrightData.length;
        if (length != 0) {
            targetStrightData.length = 0;
            targetLocusSpeed = targetStrightData.speed;
            isInOperation = true;
            if (locusElement != nullptr) {
                delete locusElement;
            }
            locusElement = new StrightLine(odmetry.getMachineVector(), length);
        }
    };
    hardware::noInterrupt(+f);
}

void updateTargetRotate() {
    auto f = []() {
        auto theta = targetRotateData.theta;
        if (theta != 0) {
            switchLedPin.toggle();
            targetRotateData.theta = 0;
            targetLocusSpeed = targetRotateData.speed;
            isInOperation = true;
            if (locusElement != nullptr) {
                delete locusElement;
            }
            auto radius = config::WheelTread / 1.4;
            radius = theta < 0 ? -radius : radius;
            locusElement = new CurveLine(odmetry.getMachineVector(), radius, (float)theta / 1000.0);
        }
    };
    hardware::noInterrupt(+f);
}

void loop() {
    if (periodicTimer.wasInterruptOccured() == true) {
        rightEncoder.load();
        leftEncoder.load();
        resetOdmetryData();
        odmetry.update();
        updateOdmetryData();
        updateTargetStright();
        updateTargetRotate();
        if (isInOperation == false) {
            updateTargetSpeed();
            vehicleController.determineOutput();
        } else {
            Guide guide = locusElement->createGuide(odmetry.getMachineVector());
            if (guide.isAvailable()) {
                auto speed = targetLocusSpeed;
                auto curvature = guide.getCurvature();
                if (curvature == 0) {
                    vehicleController.setTarget(guide.getCurvature()
                            , guide.getRemainingDistance(), speed, speed);
                } else {
                    vehicleController.setTarget(guide.getCurvature()
                            , guide.getRemainingDistance(), speed, 0);
                }
                vehicleController.determineOutput();
            } else {
                isInOperation = false;
                vehicleController.determineOutput();
            }
        }
    }
    i2cHandler->handleInterrupt();
}

int main() {
    setup();
    while (true) {
        loop();
    }
}
