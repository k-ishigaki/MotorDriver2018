#ifndef USART_H
#define USART_H

#include "hardware_implementation.hpp"

using namespace hardware::usart;

class USART {
    public:
        /**
         * Read from a receive buffer.
         *
         * Before reading from the buffer, you must confirm that data exists
         * in it.
         *
         * @return received data in receive buffer
         */
        virtual buffer_size_t read() const = 0;

        /**
         * Checks if a receive buffer has data.
         *
         * If USART#read method can return valid data, this method returns true.
         * To read from received buffer, see USART#read.
         *
         * @return true if received data exits<br>
         * false if no data exits.
         */
        virtual bool hasReceived() const = 0;

        /**
         * Write to a transmit buffer of the module.
         * 
         * Before writing to the buffer, you must confirm that it is empty.
         * {@code
         * const USART* serial = Hardware.EUSART;
         * ... (setup serial module)
         * while(serial->isTransmitting);
         * serial->write(data);
         * }
         * 
         * @param transmitting data to transmit buffer
         */
        virtual void write(buffer_size_t) const = 0;

        /**
         * Checks if a transmit buffer is empty.
         *
         * If USART#write method can be used, this method returns true.
         * To write to transmit buffer, see USART#write.
         *
         * @return true if transmit buffer is full<br>
         * false if it is empty
         */
        virtual bool isTransmitBufferEmpty() const = 0;

        /**
         * Get errors in a receiver.
         * 
         * If you want to detect which errors occured, use
         * USART_Error constants defined at Hardware.h.
         * {@code
         * const USART uasrt = Hardware.
         * USART(...);
         * uint8_t error = usart->getErrors();
         * if(error & USART_Error.OVERRUN) {
         * // overrun error occured
         * } else if (error & USART_Error.FRAMING) {
         * // framing error occured
         * }
         * }
         * 
         * @return 0 if no error occurs<br>
         * the kinds of error can be detect by hardware#uasrt#Error
         */
        virtual error_flag_t getErrors() const = 0;

        /**
         * Reset a receiver and re-enable it.
         * 
         * When USART#getErrors != 0, clearing flag may be needed.
         * Using this method, the errors can be cleared.
         */
        virtual void resetReceiver() const = 0;
};

#endif
