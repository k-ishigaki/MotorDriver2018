#ifndef HARDWARE_IMPLEMENTATION_H
#define HARDWARE_IMPLEMENTATION_H

#include <stdint.h>

namespace hardware {
    constexpr uint32_t SystemClockFrequency= F_CPU;

    template<class Fn> void doWithoutInterrupts(const Fn);

    namespace io_port {
        enum class PinMode : uint8_t;
    }

    namespace usart {
        using baudrate_size_t = unsigned long;
        using buffer_size_t = uint16_t;
        using error_flag_t = uint8_t;
        enum class Error : error_flag_t;
        enum class Mode : uint8_t;
        enum class ParityMode : uint8_t;
        enum class StopBitSize : uint8_t;
        enum class CharacterSize : uint8_t;
    }
}

#endif
