#ifndef FRAMEFORK_SERIAL_BUFFER_H
#define FRAMEFORK_SERIAL_BUFFER_H

#include "../hardware/usart.hpp"
#include "../hardware/interrupt.hpp"


class SerialBuffer {
    public:
        enum class BufferSize : uint8_t {
            D64  = 0b00111111,
            D128 = 0b01111111,
            D256 = 0b11111111,
        };

        /**
         * Serial Buffer constructor.
         *
         * @param receive/transmit buffer size<br>
         * The buffer size is selectable from BufferSize.<br>
         * The buffer size is same between receiver and transmitter.
         * @param USART interface<br>
         * The transmit/receive buffer size must be configured as 8bit.
         */
        SerialBuffer(BufferSize, const USART&, const Interrupt&);

        /**
         * Write to a transmit buffer.
         *
         * The data will be transmitted correctly as long as a buffer does not
         * overflow.
         *
         * @param data will be written to transmit buffer
         */
        void write(const uint8_t) const;

        /**
         * Get a received count from a receive buffer.
         *
         * You should check if a buffer has data before call read().
         *
         * @return data count in a receive buffer
         */
        uint8_t getReceivedCount() const;

        /**
         * Read from a receive buffer.
         *
         * Before call this method, getReceivedCount() should be called to
         * confirm a receive buffer has valid data.
         *
         * @return one byte data from a receive buffer<br>
         * If no data exists in the buffer, the return value is not specified.
         */
        uint8_t read() const;

    private:
        const uint8_t* buffer;
        const uint8_t size;
        const USART& usart;
        const Interrupt& interrupt;
};

#endif
