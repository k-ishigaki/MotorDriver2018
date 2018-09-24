#include "periodic_tiemr.hpp"
#include "log.hpp"

PeriodicTimer::PeriodicTimer(Interrupt& interrupt) {
    this->wasInterrupted = false;
    interrupt.registerHandler(new Handler(this));
    interrupt.enable();
}

bool PeriodicTimer::wasInterruptOccured() {
    auto f = [](PeriodicTimer* self) {
        if (self->wasInterrupted) {
            self->wasInterrupted = false;
            return true;
        } else {
            return false;
        }
    };
    return hardware::noInterruptWithReturn(+f, this);
}

PeriodicTimer::Handler::Handler(PeriodicTimer* outer) {
    this->outer = outer;
}

void PeriodicTimer::Handler::handleInterrupt() {
    this->outer->wasInterrupted = true;
}
