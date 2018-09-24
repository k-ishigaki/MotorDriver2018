#ifndef DEVICE_TWI_H
#define DEVICE_TWI_H

#include "device.hpp"

namespace device {
    namespace twi {
        template<uint8_t offset_> struct twi_t {
            static constexpr uint8_t offset = offset_;

            static register_t<0xB8 + offset, uint8_t> TWBR;

            template<class T> struct twsr_t : public T {
                bits_t<T, Bitpos::B3, 5> TWS;
                bits_t<T, Bitpos::B0, 2> TWPS;
            };
            static twsr_t<register_t<0xB9 + offset, uint8_t>> TWSR;

            template<class T> struct twar_t : public T {
                bits_t<T, Bitpos::B1, 7> TWA;
                bits_t<T, Bitpos::B0, 1> TWGCE;
            };
            static twar_t<register_t<0xBA + offset, uint8_t>> TWAR;

            static register_t<0xBB + offset, uint8_t> TWDR;

            template<class T> struct twcr_t : public T {
                bits_t<T, Bitpos::B7, 1> TWINT;
                bits_t<T, Bitpos::B6, 1> TWEA;
                bits_t<T, Bitpos::B5, 1> TWSTA;
                bits_t<T, Bitpos::B4, 1> TWSTO;
                bits_t<T, Bitpos::B3, 1> TWWC;
                bits_t<T, Bitpos::B2, 1> TWEN;
                bits_t<T, Bitpos::B0, 1> TWIE;
            };
            static twcr_t<register_t<0xBC + offset, uint8_t>> TWCR;

            template<class T> struct twamr_t : public T {
                bits_t<T, Bitpos::B1, 7> TWAM;
            };
            static twamr_t<register_t<0xBD + offset, uint8_t>> TWAMR;
        };

        using twi0_t = twi_t<0x00>;
        using twi1_t = twi_t<0x20>;
    }
}

#endif
