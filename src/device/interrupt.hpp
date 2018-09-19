#ifndef DEVICE_INTERRUPT_H
#define DEVICE_INTERRUPT_H

#include "device.hpp"

namespace device {
    namespace interrupt {
        template<uint8_t addr_, Bitpos pos_> struct interrupt_t
            : public register_t<addr_, uint8_t> {
                static constexpr uint8_t addr = addr_;
                static constexpr Bitpos pos = pos_;
                static bits_t<register_t<addr_, uint8_t>, pos_, 1> EI;
            };

        template<uint8_t offset> struct tx_t
            : public interrupt_t<0xC1 + offset, Bitpos::B6> {};
        using tx0_t = tx_t<0x00>;
        using tx1_t = tx_t<0x08>;

        template<uint8_t offset> struct rx_t
            : public interrupt_t<0xC1 + offset, Bitpos::B7> {};
        using rx0_t = rx_t<0x00>;
        using rx1_t = rx_t<0x08>;

        template<uint8_t offset> struct toie_t
            : public interrupt_t<0x6F + offset, Bitpos::B0> {};
        using toie1_t = toie_t<0x00>;
        using toie2_t = toie_t<0x01>;
        using toie3_t = toie_t<0x02>;
        using toie4_t = toie_t<0x03>;

        template<uint8_t offset> struct ociea_t
            : public interrupt_t<0x6F + offset, Bitpos::B1> {};
        using ocie1a_t = ociea_t<0x00>;
        using ocie2a_t = ociea_t<0x01>;
        using ocie3a_t = ociea_t<0x02>;
        using ocie4a_t = ociea_t<0x03>;

        template<uint8_t offset> struct ocieb_t
            : public interrupt_t<0x6F + offset, Bitpos::B2> {};
        using ocie1b_t = ocieb_t<0x00>;
        using ocie2b_t = ocieb_t<0x01>;
        using ocie3b_t = ocieb_t<0x02>;
        using ocie4b_t = ocieb_t<0x03>;

        using pcie0_t = interrupt_t<0x68, Bitpos::B0>;
        using pcie1_t = interrupt_t<0x68, Bitpos::B1>;
        using pcie2_t = interrupt_t<0x68, Bitpos::B2>;
        using pcie3_t = interrupt_t<0x68, Bitpos::B3>;

    }
}

#endif
