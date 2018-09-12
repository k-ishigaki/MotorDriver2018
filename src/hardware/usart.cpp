#include "usart.hpp"
#include "../device/usart.hpp"
#include "hardware.hpp"

using namespace device::usart;
using namespace hardware::usart;
using namespace hardware;

template<uint8_t offset> class USART_ : USART {
    using regs = usart_t<offset>;
    public:
        USART_(const Config& config) {
            regs::UCSRB.RXEN = 1;
            regs::UCSRB.TXEN = 1;
            regs::UBRR = SystemClockFrequency / 16UL / config.baudrate - 1;
            regs::UCSRC.UMSEL = static_cast<uint8_t>(config.mode);
            regs::UCSRC.UPM = static_cast<uint8_t>(config.parityMode);
            regs::UCSRC.USBS = static_cast<uint8_t>(config.stopBitSize);
            regs::UCSRC.UCSZ0 = static_cast<uint8_t>(config.characterSize) & 0b001;
            regs::UCSRC.UCSZ1 = (static_cast<uint8_t>(config.characterSize) & 0b010) >> 1;
            regs::UCSRB.UCSZ2 = (static_cast<uint8_t>(config.characterSize) & 0b100) >> 2;
        }

        static USART& getInstance(const Config& config) {
            static USART_<offset> instance(config);
            return instance;
        }

        buffer_size_t read() const override {
            return regs::UDR + (static_cast<buffer_size_t>(regs::UCSRB.RXB8) << 8);
        }

        bool hasReceived() const override {
            return regs::UCSRA.RXC;
        }

        void write(buffer_size_t data) const override {
            regs::UCSRB.TXB8 = data >> 8;
            regs::UDR = static_cast<uint8_t>(data & 0xFF);
        }

        bool isTransmitBufferEmpty() const override {
            return regs::UCSRA.UDRE;
        }

        void resetReceiver() const override {
            this->read();
        }

        error_flag_t getErrors() const override {
            return 0;
        }
};

const USART& hardware::usart::get0(const Config& config) {
    return USART_<0x00>::getInstance(config);
}

const USART& hardware::usart::get1(const Config& config) {
    return USART_<0x08>::getInstance(config);
}
