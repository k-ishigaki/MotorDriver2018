#include "hardware.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

extern ADConverter* getADConverter();

namespace hardware {
    namespace system_clock_prescaler {
        void configure(DivisionFactor factor) {
            CLKPR = _BV(CLKPCE);
            CLKPR = static_cast<uint8_t>(factor);
        }
    }

}

bool hardware::impl::isInterruptEnable() { return SREG & _BV(SREG_I); }
void hardware::diableGlobalInterrupt() { cli(); }
void hardware::enableGlobalInterrupt() { sei(); }
