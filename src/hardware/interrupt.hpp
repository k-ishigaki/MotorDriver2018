#ifndef HARDWARE_INTERRUPT_H
#define HARDWARE_INTERRUPT_H

#include "hardware_implementation.hpp"

class InterruptHandler {
    public:
        virtual void handleInterrupt() = 0;
};

class Interrupt {
    public:
        /**
         * Register a handler handles interrupt.
         *
         * If you call this method twice, only one handler is registered.
         * (First registerd handler is overwriten by second one.)
         * This method may takes time to register handler to interrupt
         * vector, so it is recommended calling it at initilization.
         *
         * @param interrupt handler
         */
        virtual void registerHandler(InterruptHandler*) = 0;

        /**
         * Check if the interrupt is enabled.
         *
         * @return true if enabled, false otherwise
         */
        virtual bool isEnable() const = 0;

        /**
         * Enable the interrupt.
         *
         * This method must be called after calling
         * Interrupt#registerListener.
         */
        virtual void enable() const = 0;

        /**
         * Disable the interrupt.
         *
         * You should call this method when stop the interrupt
         * because it is unable to remove registerd listener.
         */
        virtual void disable() const = 0;
};

#endif
