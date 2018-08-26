#include "hardware.hpp"
#include <avr/io.h>

extern ADConverter* getADConverter();

namespace hardware {
    namespace SystemClockPrescaler {
        void configure(DivisionFactor factor) {
            CLKPR = _BV(CLKPCE);
            CLKPR = factor;
        }
    }

//ADConverter& Hardware::getADConverter() {
//}

}
