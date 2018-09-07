#include "hardware.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

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
        cli();
        function();
        // reenables a global interrupt
        sei();
    } else {
        function();
    }
}

