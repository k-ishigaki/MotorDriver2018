#include "i2c.hpp"

#include "../device/twi.hpp"
#include "../hardware/hardware.hpp"

using namespace device::twi;
namespace hi = hardware::i2c;

template<class T> class I2CSlave_ : public I2CSlave {
    using regs = twi_t<T::offset>;
    public:
        static I2CSlave_<T>& getInstance(const hi::Config& config) {
            static I2CSlave_<T> instance;
            regs::TWAR.TWA = config.slaveAddress;
            regs::TWAR.TWGCE = static_cast<uint8_t>(config.generalCallReception);
            regs::TWCR.TWEA = 1;
            regs::TWCR.TWEN = 1;
            return instance;
        }

        enum class SlaveStatus {
            AddressReceivedWithRead = 0b10101,
            ArbitrationLostWithRead = 0b10110,
            DataTransmittedWithACK = 0b10111,
            DataTransmittedWithNACK = 0b11000,
            LastDataTransmittedWithACK = 0b11001,
            AddressReceivedWithWrite = 0b01100,
            ArbitrationLostWithWrite = 0b01101,
            GeneralCallAddressReceived = 0b01110,
            ArbitrationLostWithGeneralCallAddress = 0b01111,
            DataReceivedWithACK = 0b10000,
            DataReceivedWithNACK = 0b10001,
            GeneralCallDataReceivedWithACK = 0b10010,
            GeneralCallDataReceivedWithNACK = 0b10011,
            StopOrRestartReceived = 0b10100,
        };

        Event getEvent() override {
            if (regs::TWCR.TWINT == 0) {
                return Event::Empty;
            }
            uint8_t tmp = static_cast<uint8_t>(regs::TWSR.TWS);
            SlaveStatus request = static_cast<SlaveStatus>(tmp);
            switch (request) {
                case SlaveStatus::AddressReceivedWithRead:
                case SlaveStatus::ArbitrationLostWithRead:
                    return Event::AddressReceivedWithRead;
                case SlaveStatus::DataTransmittedWithACK:
                    return Event::DataTransmitted;
                case SlaveStatus::DataTransmittedWithNACK:
                case SlaveStatus::LastDataTransmittedWithACK:
                    return Event::LastDataTransmitted;
                case SlaveStatus::AddressReceivedWithWrite:
                case SlaveStatus::ArbitrationLostWithWrite:
                case SlaveStatus::GeneralCallAddressReceived:
                case SlaveStatus::ArbitrationLostWithGeneralCallAddress:
                    return Event::AddressReceivedWithWrite;
                case SlaveStatus::DataReceivedWithACK:
                case SlaveStatus::GeneralCallDataReceivedWithACK:
                    return Event::DataReceived;
                case SlaveStatus::DataReceivedWithNACK:
                case SlaveStatus::GeneralCallDataReceivedWithNACK:
                    return Event::LastDataReceived;
                case SlaveStatus::StopOrRestartReceived:
                    this->releaseClock();
                    return Event::Empty;
            }
            // elegal state
            return Event::Empty;
        }

        void releaseClock() override {
            regs::TWCR.TWSTA = 0;
            regs::TWCR.TWSTO = 0;
            regs::TWCR.TWEA = 1;
            regs::TWCR.TWINT = 1;
        }

        void write(uint8_t data) override {
            regs::TWDR = data;
        }

        uint8_t read() override {
            return regs::TWDR;
        }
};

I2CSlave& hi::getTwi0(const Config& config) { return I2CSlave_<twi0_t>::getInstance(config); }
I2CSlave& hi::getTwi1(const Config& config) { return I2CSlave_<twi1_t>::getInstance(config); }
