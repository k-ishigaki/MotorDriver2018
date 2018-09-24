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

Encoder::Encoder(Interrupt& interrupt, DigitalOutputPin& led) : led(led) {
    interrupt.registerHandler(new CountInterruptHandler(this));
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

Encoder::CountInterruptHandler::CountInterruptHandler(Encoder* outer) {
    this->outer = outer;
}

void Encoder::CountInterruptHandler::handleInterrupt() {
    this->outer->count++;
    this->outer->led.toggle();
}
