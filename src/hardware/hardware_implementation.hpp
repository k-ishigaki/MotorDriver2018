#ifndef HARDWARE_IMPLEMENTATION_H
#define HARDWARE_IMPLEMENTATION_H

#include <stdint.h>

namespace hardware {
    /**
     * System clock frequency [Hz].
     */
    constexpr uint32_t SystemClockFrequency= F_CPU;

    namespace io_port {
        enum PinMode : uint8_t;
    }
}
#endif
