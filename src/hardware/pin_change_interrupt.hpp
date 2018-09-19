#ifndef HARDWARE_PIN_CHANGE_INTERRUPT_H
#define HARDWARE_PIN_CHANGE_INTERRUPT_H

#include "hardware_implementation.hpp"

class PinChangeInterrupt {
    public:
        /**
         * Enable function on selected pins.
         *
         * To enabling interrupt, the corresponding Interrupt#enable must be also called.
         *
         * @param pin positions enabling interrupt
         */
        virtual void enable(uint8_t) = 0;

        /**
         * Disable interrupt on selected pins.
         *
         * To disable interrupt, the corresponding Interrupt#disable must be also called.
         *
         * @param pin positions diabling interrupt
         */
        virtual void disable(uint8_t) = 0;
};

#endif
