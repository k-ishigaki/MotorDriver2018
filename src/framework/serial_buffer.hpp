#ifndef FRAMEFORK_SERIAL_BUFFER_H
#define FRAMEFORK_SERIAL_BUFFER_H

#include "../basic/ring_buffer.hpp"
#include "../hardware/usart.hpp"
#include "../hardware/interrupt.hpp"


class SerialBuffer {
    public:
        /**
         * Serial Buffer constructor.
         *
         * @param receive/transmit buffer size<br>
         * The buffer size is selectable from BufferSize.<br>
         * The buffer size is same between receiver and transmitter.
         * @param USART interface<br>
         * The transmit/receive buffer size must be configured as 8bit.
         * @param receive interrupt interface
         * @param transmit interrupt interface
         */
        SerialBuffer(RingBuffer::Size, const USART&, const Interrupt&, const Interrupt&);

        /**
         * Write to a transmit buffer.
         *
         * The data will be transmitted correctly as long as a buffer does not
         * overflow.
         *
         * @param data will be written to transmit buffer
         */
        void write(const uint8_t);

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
        uint8_t read();

    private:
        class ReceiveHandler : public InterruptHandler {
            public:
                ReceiveHandler(SerialBuffer*);
                void handleInterrupt();
            private:
                SerialBuffer* outer;
        };
        class TransmitHandler : public InterruptHandler {
            public:
                TransmitHandler(SerialBuffer*);
                void handleInterrupt();
            private:
                SerialBuffer* outer;
        };
        RingBuffer* receiveBuffer;
        RingBuffer* transmitBuffer;
        const USART& usart;
        const Interrupt& receiveInterrupt;
        const Interrupt& transmitInterrupt;
};

#endif
