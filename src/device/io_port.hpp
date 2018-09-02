#ifndef DEVICE_IO_PORT_H
#define DEVICE_IO_PORT_H

#include "device.hpp"

namespace device {
    namespace io_port {
        template<uint8_t offset> struct io_port_t {
            static register_t<0x23 + offset, uint8_t> PIN;
            static register_t<0x24 + offset, uint8_t> DDR;
            static register_t<0x25 + offset, uint8_t> PORT;
        };
    }
}

#endif
