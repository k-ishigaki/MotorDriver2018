#include "encoder.hpp"

namespace {
    constexpr float pi = 3.1415926535897932384626433832795;
    constexpr float tireDiameter = 58;
    constexpr float tireCircumference = tireDiameter * pi;
    constexpr float gearRatio = 40.5;
    constexpr float numberOfCounts = 60;
    constexpr float calcLength(float count) {
        return count * tireCircumference / (gearRatio * numberOfCounts);
    }
}

Encoder::Encoder(Interrupt& interrupt, DigitalInputPin& inputPin, DigitalOutputPin& led) {
    interrupt.registerHandler(new CountInterruptHandler(this, inputPin, led));
    interrupt.enable();
}

void Encoder::load() {
    auto f = [](Encoder* self) {
        auto result = self->count;
        self->count = 0;
        return result;
    };
    this->integratedLength = calcLength(hardware::noInterruptWithReturn(+f, this));
}

float Encoder::getIntegratedLength() const {
    return this->integratedLength;
}

Encoder::CountInterruptHandler::CountInterruptHandler(Encoder* outer, DigitalInputPin& inputPin, DigitalOutputPin& led) : inputPin(inputPin), led(led) {
    this->outer = outer;
    this->currentState = this->inputPin.read();
    this->led.write(this->currentState);
}

void Encoder::CountInterruptHandler::handleInterrupt() {
    if (this->currentState != this->inputPin.read()) {
        // single count
        this->currentState = !this->currentState;
        this->outer->count++;
        this->led.toggle();
    } else {
        // invalid count, do nothing
    }
}
