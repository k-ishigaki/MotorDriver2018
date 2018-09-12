#include "serial_buffer.hpp"

SerialBuffer::SerialBuffer(
        RingBuffer::Size size,
        const USART& usart,
        Interrupt& receiveInterrupt,
        Interrupt& transmitInterrupt)
    : usart(usart),
    receiveInterrupt(receiveInterrupt),
    transmitInterrupt(transmitInterrupt) {
        this->transmitBuffer = new RingBuffer(size);
        this->receiveBuffer = new RingBuffer(size);
        this->transmitInterrupt.registerHandler(new TransmitHandler(this));
        this->receiveInterrupt.registerHandler(new ReceiveHandler(this));
        // The transmit interrupt will be enabled after write to transmit buffer
        this->receiveInterrupt.enable();
}

void SerialBuffer::write(uint8_t data) {
    auto f = [](SerialBuffer* self, uint8_t data) {
        if (self->transmitInterrupt.isEnable() == false) {
            self->usart.write(data);
            self->transmitInterrupt.enable();
        } else {
            self->transmitBuffer->push(data);
        }
    };
    hardware::noInterrupt(+f, this, data);
}

uint8_t SerialBuffer::getReceivedCount() {
    auto f = [](SerialBuffer* self) {
        return self->receiveBuffer->getAvailableSize();
    };
    return hardware::noInterruptWithReturn(+f, this);
}

uint8_t SerialBuffer::read() {
    auto f = [](SerialBuffer* self) {
        auto availableSize = self->receiveBuffer->getAvailableSize();
        return availableSize > 0 ? self->receiveBuffer->pop() : 0;
    };
    return hardware::noInterruptWithReturn(+f, this);
}

SerialBuffer::ReceiveHandler::ReceiveHandler(SerialBuffer* outer) { this->outer = outer; }

void SerialBuffer::ReceiveHandler::handleInterrupt() {
    uint8_t result = this->outer->usart.read();
    this->outer->receiveBuffer->push(result);
}

SerialBuffer::TransmitHandler::TransmitHandler(SerialBuffer* outer) { this->outer = outer; }

void SerialBuffer::TransmitHandler::handleInterrupt() {
    if (this->outer->transmitBuffer->getAvailableSize() == 0) {
        this->outer->transmitInterrupt.disable();
    } else {
        this->outer->usart.write(this->outer->transmitBuffer->pop());
    }
}
