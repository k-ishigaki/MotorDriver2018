#ifndef HARDWARE_PWM_H
#define HARDWARE_PWM_H

#include "hardware_implementation.hpp"

class PWM {
    public:
        /**
         * Set output Mode of PWM
         *
         * @param output mode depends on device
         */
        virtual void setOutputMode(hardware::pwm::OutputMode);

        /**
         * Set duty cycle count.
         *
         * @param duty cycle count
         */
        virtual void setDutyCycle(uint16_t) = 0;
};

#endif
