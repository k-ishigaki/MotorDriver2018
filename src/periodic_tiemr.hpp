#ifndef PERIODIC_TIMER_H
#define PERIODIC_TIMER_H

#include "hardware/interrupt.hpp"

class PeriodicTimer {
    public:
        PeriodicTimer(Interrupt&);
        bool wasInterruptOccured();
    private:
        bool wasInterrupted;
        class Handler : public InterruptHandler {
            public:
                Handler(PeriodicTimer* outer);
                void handleInterrupt() override;
            private:
                PeriodicTimer* outer;
        };
};

#endif
