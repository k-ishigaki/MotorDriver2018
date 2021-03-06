#include "ring_buffer.hpp"
#include <math.h>

namespace {
    template<class T>
        constexpr T pow(T base, T exp) {
            return exp <= 0 ? 1
                :  exp == 1 ? base
                :  base * pow(base, exp-1);
        }
}

RingBuffer::RingBuffer(const Size size)
    //: maskBits(static_cast<uint8_t>(static_cast<uint16_t>(0x00FF) << static_cast<uint8_t>(size)) >> 8) {
    : maskBits(0b00111111) {
        //this->array = new uint8_t[static_cast<uint8_t>(pow(2, static_cast<uint8_t>(size)))];
        this->array = new uint8_t[64];
        this->readIndex = 0;
        this->availableDataSize = 0;
}

void RingBuffer::push(const uint8_t data) {
    array[(this->readIndex + this->availableDataSize++) & this->maskBits] = data;
    this->availableDataSize &= this->maskBits;
}

uint8_t RingBuffer::pop() {
    if (this->availableDataSize == 0) {
        return 0;
    }
    this->availableDataSize--;
    uint8_t result = array[this->readIndex++];
    this->readIndex &= this->maskBits;
    return result;
}

uint8_t RingBuffer::getAvailableSize() const {
    return this->availableDataSize;
}
