#ifndef HARDWARE_TIMER_H
#define HARDWARE_TIMER_H

#include "hardware_implementation.hpp"

class Timer {
    public:
        /**
         * Get counter value of timer module.
         *
         * At several device, countup operation is stoped temporarily to
         * get accurate counter value.
         * So you should pay attention to the temporary timer stop.
         *
         * @return counter value of timer module<br>
         * (8bit timer) 0~255<br>
         * (16bit timer) 0~65535
         */
        virtual uint16_t getCount() = 0;

        /**
         * Set counter value of timer module.
         *
         * At several device, countup operation is stoped temporarily to
         * set accurate counter value.
         * So you should pay attention to the temporary timer stop.
         *
         * @param counter value of timer module<br>
         * (8bit timer) 0~255<br>
         * (16bit timer) 0~65535
         */
        virtual void setCount(uint16_t) = 0;

        /**
         * Start Counting.
         *
         * When timer module constructor called, the counting is disabled.
         * So this method is needed to start timer operation.
         * But timer counting is always enabled at timer module which counting
         * cannot be stoped. (see datasheet)
         */
        virtual void start() = 0;

        /**
         * Stop Counting.
         *
         * If you stop timer counting operation temporarily, use this method.
         * TimerModule#start method can be used for resumption operation.
         * But timer counting is always enabled at timer module which counting
         * cannot be stoped. (see datasheet)
         */
        virtual void stop() = 0;
};

#endif
