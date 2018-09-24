#ifndef HARDWARE_I2C_H
#define HARDWARE_I2C_H

#include "hardware_implementation.hpp"

class I2CSlave {
    public:
        enum class Event {
            Empty,
            AddressReceivedWithRead,
            DataTransmitted,
            LastDataTransmitted,
            AddressReceivedWithWrite,
            DataReceived,
            LastDataReceived,
        };
        /**
         * Get software operation request from hardware.
         *
         * If no event has been occured, Event#empty will be returned.
         * The request will be cleared when releaseClock() is called.
         *
         * @return software operation request
         */
        virtual Event getEvent() = 0;

        enum class NextOperation {
            FinishTransferring,
            TransferLastByte,
            TransferMoreThan1Byte,
        };
        /**
         * Release CLK clock and start hardware operation.
         *
         * @param next operation for hardware
         */
        virtual void releaseClock(NextOperation) = 0;

        /**
         * Write a data to transmit buffer.
         *
         * The written data is transmitted when releaseClock() called. 
         *
         * @param a data written to buffer
         */
        virtual void write(uint8_t) = 0;

        /**
         * Read a data from receive buffer.
         *
         * The received data must be read before releaseClock() called.
         *
         * @return a receive data
         */
        virtual uint8_t read() = 0;
};

#endif
