#include "shared_interrupt.hpp"

SharedInterrupt::SharedInterrupt(Interrupt& interrupt, uint8_t numberOfAddable)
    : interrupt(interrupt) {
        handlers = new InterruptHandler*[numberOfAddable];
        indexOfHandlers = 0;
    }

void SharedInterrupt::addInterruptHandler(InterruptHandler* handler) {
    handlers[indexOfHandlers++] = handler;
}

SharedInterrupt::SharedInterruptHandler::SharedInterruptHandler(SharedInterrupt* outer) {
    this->outer = outer;
}

void SharedInterrupt::SharedInterruptHandler::handleInterrupt() {
    for (int8_t i = 0; i < outer->indexOfHandlers - 1; i++) {
        outer->handlers[i]->handleInterrupt();
    }
}
