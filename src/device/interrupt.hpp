#ifndef DEVICE_INTERRUPT_H
#define DEVICE_INTERRUPT_H

#include "device.hpp"

namespace device {
    namespace interrupt {
        template<uint8_t addr_, Bitpos pos_> struct interrupt_t
            : public register_t<addr_, uint8_t> {
                static constexpr uint8_t addr = addr_;
                static constexpr Bitpos pos = pos_;
                bits_t<register_t<addr_, uint8_t>, pos_, 1> EI;
            };

        template<uint8_t offset> struct tx_t
            : public interrupt_t<0xC1 + offset, Bitpos::B6> {};
        using tx0_t = tx_t<0x00>;
        using tx1_t = tx_t<0x08>;

        template<uint8_t offset> struct rx_t
            : public interrupt_t<0xC1 + offset, Bitpos::B7> {};
        using rx0_t = rx_t<0x00>;
        using rx1_t = rx_t<0x08>;
    }
}

#endif
