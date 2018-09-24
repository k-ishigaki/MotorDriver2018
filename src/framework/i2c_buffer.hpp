#ifndef FRAMEWORK_I2C_BUFFER_H
#define FRAMEWORK_I2C_BUFFER_H

#include "../hardware/i2c.hpp"
#include "../hardware/interrupt.hpp"

class I2CBuffer {
    public:
        I2CBuffer(I2CSlave&, Interrupt&, uint8_t, uint8_t);

        /**
         * Register data of i2c slave.
         *
         * @param address
         * @param target data pointer
         * @param data size
         */
        void registerData(uint8_t, uint8_t*, uint8_t);

    private:
        const uint8_t maxAddress;
        const uint8_t bufferSize;
        uint8_t* identifiers;
        uint8_t** pointers;
        class SlaveInterruptHandler : public InterruptHandler {
            public:
                SlaveInterruptHandler(I2CBuffer* outer, I2CSlave&, uint8_t);
                void handleInterrupt() override;
            private:
                I2CBuffer* outer;
                I2CSlave& i2cSlave;
                uint8_t* buffer;
                uint8_t currentAddress;
                uint8_t currentIndex;
        };
};

#endif
