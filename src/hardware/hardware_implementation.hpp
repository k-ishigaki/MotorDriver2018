#ifndef HARDWARE_IMPLEMENTATION_H
#define HARDWARE_IMPLEMENTATION_H

#include <stdint.h>

namespace hardware {
    constexpr uint32_t SystemClockFrequency= F_CPU;

    namespace impl {
        bool isInterruptEnable();
    }

    void diableGlobalInterrupt();
    void enableGlobalInterrupt();

    template<typename ... Args> void noInterrupt(
            void (*handler)(Args ...), Args ... args) {
        if (impl::isInterruptEnable() == false) {
            handler(args...);
            return;
        }
        diableGlobalInterrupt();
        handler(args...);
        enableGlobalInterrupt();
    }

    template<typename T, typename ... Args> T noInterruptWithReturn(
            T (*handler)(Args ...), Args ... args) {
        if (impl::isInterruptEnable() == false) {
            return handler(args...);
        }
        diableGlobalInterrupt();
        T result = handler(args...);
        enableGlobalInterrupt();
        return result;
    }


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
