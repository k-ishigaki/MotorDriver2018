#include "i2c_buffer.hpp"

I2CBuffer::I2CBuffer(I2CSlave& i2cSlave, Interrupt& interrupt, uint8_t maxAddress, uint8_t bufferSize)
    : maxAddress(maxAddress), bufferSize(bufferSize) {
        this->identifiers = new uint8_t[maxAddress];
        this->pointers = new uint8_t*[maxAddress];
        interrupt.registerHandler(new SlaveInterruptHandler(this, i2cSlave, bufferSize));
        interrupt.enable();
}

void I2CBuffer::registerData(uint8_t address, uint8_t* pointer, uint8_t dataSize) {
    if (address > this->maxAddress) {
        //log_e("address is larger than maxAddress");
    }
    if (dataSize > this->bufferSize) {
        //log_e("dataSize is larger than bufferSize");
    }
    this->identifiers[address] = dataSize;
    this->pointers[address] = pointer;
}

I2CBuffer::SlaveInterruptHandler::SlaveInterruptHandler(I2CBuffer* outer, I2CSlave& i2cSlave, uint8_t bufferSize) : i2cSlave(i2cSlave) {
    this->outer = outer;
    this->buffer = new uint8_t[bufferSize];
    this->currentAddress = 0;
    this->currentIndex = 0;
}

void I2CBuffer::SlaveInterruptHandler::handleInterrupt() {
    switch (this->i2cSlave.getEvent()) {
        case I2CSlave::Event::Empty:
            return;
        case I2CSlave::Event::AddressReceivedWithRead:
            //log_i("AddressReceivedR");
            if (this->currentAddress == 0) {
                //log_e("not addressed");
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                return;
            }
            switch (this->outer->identifiers[this->currentAddress]) {
                case 0:
                    //log_e("ellegal address");
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                    return;
                case 1:
                    // buffer is not used because of single transmission
                    this->i2cSlave.write((this->outer->pointers[this->currentAddress])[0]);
                    this->currentAddress = 0;
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferLastByte);
                    return;
                default:
                    auto pointer = this->outer->pointers[this->currentAddress];
                    auto bufferSize = this->outer->identifiers[this->currentAddress];
                    //log_i("currentIndex = %d", currentIndex);
                    for (uint8_t i = 0; i < bufferSize; i++) {
                        this->buffer[i] = pointer[i];
                    }
                    this->i2cSlave.write(this->buffer[this->currentIndex++]);
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferMoreThan1Byte);
                    return;
            }
            // cannot be reached
            return;
        case I2CSlave::Event::DataTransmitted:
        case I2CSlave::Event::LastDataTransmitted:
            if (this->currentIndex == this->outer->identifiers[this->currentAddress]) {
                this->currentIndex = 0;
                this->currentAddress = 0;
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                return;
            }
            this->i2cSlave.write(this->buffer[this->currentIndex++]);
            if (this->currentIndex > this->outer->identifiers[this->currentAddress] - 1) {
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferLastByte);
            } else {
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferMoreThan1Byte);
            }
            // cannot be reached
            return;
        case I2CSlave::Event::AddressReceivedWithWrite:
            //log_i("AddressReceivedW");
            if (this->currentAddress == 0) {
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferMoreThan1Byte);
                return;
            }
            switch (this->outer->identifiers[this->currentAddress]) {
                case 0:
                    //log_e("ellegal address");
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                    return;
                case 1:
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferLastByte);
                    return;
                default:
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferMoreThan1Byte);
                    return;
            }
            // cannot be reached
            return;
        case I2CSlave::Event::DataReceived:
        case I2CSlave::Event::LastDataReceived:
            //log_i("DataReceived");
            if (this->currentAddress == 0) {
                // address reception case
                auto receivedAddress = this->i2cSlave.read();
                if (receivedAddress > this->outer->maxAddress) {
                    //log_e("receivedAddress > maxAddress");
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                    return;
                }
                this->currentAddress = receivedAddress;
                this->currentIndex = 0;
                this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
            } else {
                // data reception case
                auto receivedData = this->i2cSlave.read();
                this->buffer[this->currentIndex++] = receivedData;
                auto bufferSize = this->outer->identifiers[this->currentAddress];
                if (this->currentIndex > bufferSize - 1) {
                    this->currentIndex = 0;
                    auto pointer = this->outer->pointers[this->currentAddress];
                    for (uint8_t i = 0; i < bufferSize; i++) {
                        pointer[i] = this->buffer[i];
                    }
                    this->currentAddress = 0;
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                } else if (this->currentIndex == bufferSize - 1) {
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::FinishTransferring);
                } else {
                    this->i2cSlave.releaseClock(I2CSlave::NextOperation::TransferMoreThan1Byte);
                }
            }
            // cannot be reached
            return;
    }
}
