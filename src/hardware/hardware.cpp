#include "hardware.hpp"
#include <avr/io.h>

extern ADConverter* getADConverter();

namespace hardware {
    namespace SystemClockPrescaler {
        void configure(DivisionFactor factor) {
            CLKPR = _BV(CLKPCE);
            CLKPR = static_cast<uint8_t>(factor);
        }
    }

//ADConverter& Hardware::getADConverter() {
//}

}

template<class Fn> void hardware::doWithoutInterrupts(const Fn function) {
    if (SREG & _BV(SREG_I)) {
        // disables a global interrupt
        SREG &= ~_BV(SREG_I);
        function();
        // reenables a global interrupt
        SREG |= _BV(SREG_I);
    } else {
        function();
    }
}

