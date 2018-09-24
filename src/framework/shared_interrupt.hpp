#ifndef FRAMEWORK_SHARED_INTERRUPT_H
#define FRAMEWORK_SHARED_INTERRUPT_H

#include "../hardware/interrupt.hpp"

class SharedInterrupt {
    public:
        /**
         * Construct shard interrupt.
         *
         * This class is used for sharing intrrupt vector between each interrupt handler.
         * Prease take care that the actual interrupt is always enabled after
         * this constructor and cannot be disabled by this instance.
         *
         * @param interrupt interface shared by others
         * @param number of handlers witch is addable
         */
        SharedInterrupt(Interrupt&, uint8_t);

        /**
         * Add interrupt handler to interrupt vector.
         *
         * The added handlers are called in order when interrupt occurs.
         * The maximal number of handlers is determined by constructor.
         *
         * @param handler which registerd to the interrupt vector
         */
        void addInterruptHandler(InterruptHandler* handler);

    private:
        Interrupt& interrupt;
        InterruptHandler** handlers;
        int8_t indexOfHandlers;
        class SharedInterruptHandler : public InterruptHandler {
            public:
                SharedInterruptHandler(SharedInterrupt* outer);
                void handleInterrupt() override;
            private:
                SharedInterrupt* outer;
        };
};

#endif
