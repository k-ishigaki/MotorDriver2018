#ifndef BASIC_RING_BUFFER_H
#define BASIC_RING_BUFFER_H

#include <stdio.h>

class RingBuffer {
    public:
        enum class Size {
            D32 = 5,
            D64 = 6,
            D128 = 7,
            D256 = 8,
        };

        /**
         * Ring Buffer constructor.
         *
         * @param buffer size
         */
        RingBuffer(const Size);

        /**
         * Push one data to a buffer.
         *
         * @param data pushed to the buffer
         */
        void push(const uint8_t data);

        /**
         * Pop one data from a buffer.
         *
         * getAvailableSize() is must be called before this method not to
         * pop nil data.
         * If no data available in the buffer, the return value is not specified.
         *
         * @return data poped from the buffer
         */
        uint8_t pop();

        /**
         * Get available data size in a buffer.
         *
         * Note that a return value is limited to range from 0 to (a buffer size - 1).
         *
         * @return data size which is available in the buffer
         */
        uint8_t getAvailableSize() const;

    private:
        const uint8_t maskBits;
        uint8_t* array;
        uint8_t readIndex;
        uint8_t availableDataSize;
};

#endif
