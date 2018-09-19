#include "pin_change_interrupt.hpp"
#include "../device/pin_change_interrupt.hpp"
#include "hardware.hpp"

using namespace device::pin_change_interrupt;
namespace hp = hardware::pin_change_interrupt;

template<class T> class PinChangeInterrupt_ : public PinChangeInterrupt {
    using regs = device::pin_change_interrupt::pci_t<T::offset>;
    public:
        static PinChangeInterrupt_<T>& getInstance() {
            static PinChangeInterrupt_<T> instance;
            return instance;
        }

        void enable(uint8_t pins) override {
            regs::PCMSK |= pins;
        }

        void disable(uint8_t pins) override {
            regs::PCMSK &= ~pins;
        }
};

PinChangeInterrupt& hp::getPCI0() { return PinChangeInterrupt_<pci0_t>::getInstance(); }
PinChangeInterrupt& hp::getPCI1() { return PinChangeInterrupt_<pci1_t>::getInstance(); }
PinChangeInterrupt& hp::getPCI2() { return PinChangeInterrupt_<pci2_t>::getInstance(); }
PinChangeInterrupt& hp::getPCI3() { return PinChangeInterrupt_<pci3_t>::getInstance(); }
