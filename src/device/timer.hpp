#ifndef DEVICE_TIMER_H
#define DEVICE_TIMER_H

#include "device.hpp"

namespace device {
    namespace timer {
        template<uint8_t offset_> struct timer_t {
            static constexpr uint8_t offset = offset_;

            template<class T> struct tccra_t : public T {
                bits_t<T, Bitpos::B6, 2> COMA;
                bits_t<T, Bitpos::B4, 2> COMB;
                bits_t<T, Bitpos::B1, 1> WGM_1;
                bits_t<T, Bitpos::B0, 0> WGM_0;
            };
            static tccra_t<register_t<0x80 + offset_, uint8_t>> TCCRA;

            template<class T> struct tccrb_t : public T {
                bits_t<T, Bitpos::B7, 1> ICNC;
                bits_t<T, Bitpos::B6, 1> ICES;
                bits_t<T, Bitpos::B4, 1> WGM_3;
                bits_t<T, Bitpos::B3, 1> WGM_2;
                bits_t<T, Bitpos::B0, 3> CS;
            };
            static tccrb_t<register_t<0x81 + offset_, uint8_t>> TCCRB;

            template<class T> struct tccrc_t : public T {
                bits_t<T, Bitpos::B7, 1> FOCA;
                bits_t<T, Bitpos::B6, 1> FOCB;
            };
            static tccrc_t<register_t<0x82 + offset_, uint8_t>> TCCRC;

            static register_t<0x84 + offset_, uint16_t> TCNT;

            static register_t<0x86 + offset_, uint16_t> ICR;

            static register_t<0x88 + offset_, uint16_t> OCRA;

            static register_t<0x8A + offset_, uint16_t> OCRB;
        };

        using timer1_t = timer_t<0x00>;
        using timer3_t = timer_t<0x10>;
        using timer4_t = timer_t<0x20>;
    }
}

#endif
