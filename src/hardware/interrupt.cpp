#include "interrupt.hpp"
#include "../device/interrupt.hpp"
#include "hardware.hpp"
#include <avr/interrupt.h>

using namespace device::interrupt;

namespace hi = hardware::interrupt;

template<class T> class Interrupt_ : public Interrupt {
    using reg = interrupt_t<T::addr, T::pos>;
    public:
        static Interrupt_<T>& getInstance() {
            static Interrupt_<T> instance;
            return instance;
        }

        void registerHandler(InterruptHandler* handler) override {
            auto f = [](Interrupt_* self, InterruptHandler* handler) {
                    self->handler = handler;
            };
            hardware::noInterrupt(+f, this, handler);
        }

        bool isEnable() const override { return reg::EI; }

        void enable() const override { reg::EI = 1; }

        void disable() const override { reg::EI = 0; }

        void handleInterrupt() {
            if (handler != nullptr) { handler->handleInterrupt(); }
        }
            
    private:
        InterruptHandler* handler = nullptr;
};

ISR(USART0_TX_vect) { Interrupt_<tx0_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getTx0() { return Interrupt_<tx0_t>::getInstance(); }

ISR(USART0_RX_vect) { Interrupt_<rx0_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getRx0() { return Interrupt_<rx0_t>::getInstance(); }

ISR(USART1_TX_vect) { Interrupt_<tx1_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getTx1() { return Interrupt_<tx1_t>::getInstance(); }

ISR(USART1_RX_vect) { Interrupt_<rx1_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getRx1() { return Interrupt_<rx1_t>::getInstance(); }

ISR(TIMER1_OVF_vect) { Interrupt_<toie1_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getTimer1Ovf() { return Interrupt_<toie1_t>::getInstance(); }

ISR(TIMER1_COMPA_vect) { Interrupt_<ocie1a_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC1A() { return Interrupt_<ocie1a_t>::getInstance(); }

ISR(TIMER1_COMPB_vect) { Interrupt_<ocie1b_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC1B() { return Interrupt_<ocie1b_t>::getInstance(); }

ISR(TIMER3_OVF_vect) { Interrupt_<toie3_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getTimer3Ovf() { return Interrupt_<toie3_t>::getInstance(); }

ISR(TIMER3_COMPA_vect) { Interrupt_<ocie3a_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC3A() { return Interrupt_<ocie3a_t>::getInstance(); }

ISR(TIMER3_COMPB_vect) { Interrupt_<ocie3b_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC3B() { return Interrupt_<ocie3b_t>::getInstance(); }

ISR(TIMER4_OVF_vect) { Interrupt_<toie4_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getTimer4Ovf() { return Interrupt_<toie4_t>::getInstance(); }

ISR(TIMER4_COMPA_vect) { Interrupt_<ocie4a_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC4A() { return Interrupt_<ocie4a_t>::getInstance(); }

ISR(TIMER4_COMPB_vect) { Interrupt_<ocie4b_t>::getInstance().handleInterrupt(); }
Interrupt& hi::getOC4B() { return Interrupt_<ocie4b_t>::getInstance(); }
