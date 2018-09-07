#include "serial_buffer.hpp"

class RingBuffer {
    RingBuffer(SerialBuffer::BufferSize size) {
    }
    void push(const uint8_t data) const {
    }

    const uint8_t pop() const {
        return 0;
    }
};

SerialBuffer::SerialBuffer(BufferSize size, const USART& usart, const Interrupt& interrupt)
    : size(static_cast<uint8_t>(size)), usart(usart), interrupt(interrupt) {
        this->buffer = new uint8_t[this->size];
        this->interrupt.enable();
}

void SerialBuffer::write(uint8_t data) const {
}
