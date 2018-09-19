#ifndef DEVICE_PIN_CHANGE_INTERRUPT_H
#define DEVICE_PIN_CHANGE_INTERRUPT_H

#include "device.hpp"

namespace device {
    namespace pin_change_interrupt {
        template<uint8_t offset_> struct pci_t {
            static constexpr uint8_t offset = offset_;
            static register_t<0x6B + offset_, uint8_t> PCMSK;
        };
        using pci0_t = pci_t<0x00>;
        using pci1_t = pci_t<0x01>;
        using pci2_t = pci_t<0x02>;
        using pci3_t = pci_t<0x08>;
    }
}

#endif
