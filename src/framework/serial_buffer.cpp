#include "serial_buffer.hpp"

SerialBuffer::SerialBuffer(RingBuffer::Size size, const USART& usart, const Interrupt& receiveInterrupt, const Interrupt& transmitInterrupt)
    : usart(usart),
    receiveInterrupt(receiveInterrupt),
    transmitInterrupt(transmitInterrupt) {
        this->transmitBuffer = new RingBuffer(size);
        this->receiveBuffer = new RingBuffer(size);
        this->receiveInterrupt.enable();
}

void SerialBuffer::write(uint8_t data) {
    if (this->usart.isTransmitBufferEmpty() == true) {
        this->usart.write(data);
        this->transmitInterrupt.enable();
        return;
    }
    this->transmitBuffer->push(data);
    return;
}

uint8_t SerialBuffer::getReceivedCount() const {
    return this->receiveBuffer->getAvailableSize();
}

uint8_t SerialBuffer::read() {
    if (this->receiveBuffer->getAvailableSize() == 0) {
        return 0;
    }
    return this->receiveBuffer->pop();
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
        return;
    }
    this->outer->usart.write(this->outer->transmitBuffer->pop());
}
