#include "i2c_buffer.hpp"

#include "../log.hpp"

I2CBuffer::I2CBuffer(I2CSlave& i2cSlave, Interrupt& interrupt, uint8_t maxAddress, uint8_t bufferSize)
    : maxAddress(maxAddress), bufferSize(bufferSize) {
        this->identifiers = new uint8_t[maxAddress];
        this->pointers = new uint8_t*[maxAddress];
        interrupt.registerHandler(new SlaveInterruptHandler(this, i2cSlave, bufferSize));
        interrupt.enable();
}

void I2CBuffer::registerData(uint8_t address, uint8_t* pointer, uint8_t dataSize) {
    if (address > this->maxAddress) {
        log_e("address is larger than maxAddress");
    }
    if (dataSize > this->bufferSize) {
        log_e("dataSize is larger than bufferSize");
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
            if (this->currentAddress == 0) {
                log_e("subaddress has not been received");
                break;
            }
            if (this->outer->identifiers[this->currentAddress] == 0) {
                log_e("no subaddress buffer exists");
                this->currentAddress = 0;
                break;
            }
            if (this->outer->identifiers[this->currentAddress] == 1) {
                // normal case : single transmission using no buffer
                this->i2cSlave.write((this->outer->pointers[this->currentAddress])[0]);
                this->currentAddress = 0;
            } else {
                // normal case : multiple transmission using buffer
                auto pointer = this->outer->pointers[this->currentAddress];
                auto bufferSize = this->outer->identifiers[this->currentAddress];
                //log_i("currentIndex = %d", currentIndex);
                for (uint8_t i = 0; i < bufferSize; i++) {
                    this->buffer[i] = pointer[i];
                }
                this->currentIndex = 0;
                this->i2cSlave.write(this->buffer[this->currentIndex++]);
            }
            break;
        case I2CSlave::Event::DataTransmitted:
        case I2CSlave::Event::LastDataTransmitted:
            if (this->currentIndex == this->outer->identifiers[this->currentAddress]) {
                // normal case : last data has been transmitted
                this->currentAddress = 0;
            } else {
                // normal case : transmission using buffer
                this->i2cSlave.write(this->buffer[this->currentIndex++]);
            }
            break;
        case I2CSlave::Event::AddressReceivedWithWrite:
            if (this->currentAddress == 0) {
                // normal case : subaddress will be received
                break;
            }
            if (this->outer->identifiers[this->currentAddress] == 0) {
                log_e("no subaddress buffer exists");
                this->currentAddress = 0;
                break;
            }
            // normal case : subaddress has been already received and buffer exists
            // do nothing
            break;
        case I2CSlave::Event::DataReceived:
        case I2CSlave::Event::LastDataReceived:
            if (this->currentAddress == 0) {
                // address reception case
                auto receivedAddress = this->i2cSlave.read();
                if (receivedAddress > this->outer->maxAddress) {
                    log_e("subaddress is out of bounds");
                    break;
                }
                // normal case : subaddress has been received
                this->currentAddress = receivedAddress;
                this->currentIndex = 0;
            } else {
                auto receivedData = this->i2cSlave.read();
                this->buffer[this->currentIndex++] = receivedData;
                auto bufferSize = this->outer->identifiers[this->currentAddress];
                if (this->currentIndex == bufferSize) {
                    // normal case : last data has been received
                    this->currentIndex = 0;
                    auto pointer = this->outer->pointers[this->currentAddress];
                    for (uint8_t i = 0; i < bufferSize; i++) {
                        pointer[i] = this->buffer[i];
                    }
                    this->currentAddress = 0;
                } else {
                    // normal case : data has been received
                }
            }
            break;
    }
    this->i2cSlave.releaseClock();
}
