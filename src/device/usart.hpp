#ifndef DEVICE_USART_H
#define DEVICE_USART_H

#include "device.hpp"

namespace device {
    namespace usart {
        template<uint8_t offset> struct usart_t {
            static register_t<0xC6 + offset, uint8_t> UDR;

            template<class T> struct ucsra_t : public T {
                bits_t<T, Bitpos::B7, 1> RXC;
                bits_t<T, Bitpos::B6, 1> TXC;
                bits_t<T, Bitpos::B5, 1> UDRE;
                bits_t<T, Bitpos::B4, 1> FE;
                bits_t<T, Bitpos::B3, 1> DOR;
                bits_t<T, Bitpos::B2, 1> UPE;
                bits_t<T, Bitpos::B1, 1> U2X;
                bits_t<T, Bitpos::B0, 1> MPCM;
            };
            static ucsra_t<register_t<0xC0 + offset, uint8_t>> UCSRA;

            template<class T> struct ucsrb_t : public T {
                bits_t<T, Bitpos::B7, 1> RXICE;
                bits_t<T, Bitpos::B6, 1> TXICE;
                bits_t<T, Bitpos::B5, 1> UDRIE;
                bits_t<T, Bitpos::B4, 1> RXEN;
                bits_t<T, Bitpos::B3, 1> TXEN;
                bits_t<T, Bitpos::B2, 1> UCSZ2;
                bits_t<T, Bitpos::B1, 1> RXB8;
                bits_t<T, Bitpos::B0, 1> TXB8;
            };
            static ucsrb_t<register_t<0xC1 + offset, uint8_t>> UCSRB;

            template<class T> struct ucsrc_t : public T {
                bits_t<T, Bitpos::B6, 2> UMSEL;
                bits_t<T, Bitpos::B4, 2> UPM;
                bits_t<T, Bitpos::B3, 1> USBS;
                bits_t<T, Bitpos::B2, 1> UCSZ1;
                bits_t<T, Bitpos::B2, 1> UDORD;
                bits_t<T, Bitpos::B1, 1> UCSZ0;
                bits_t<T, Bitpos::B1, 1> UCPHA;
                bits_t<T, Bitpos::B0, 1> UCPOL;
            };
            static ucsrc_t<register_t<0xC2 + offset, uint8_t>> UCSRC;

            static register_t<0xC4 + offset, uint16_t> UBRR;
        };
    }
}

#endif
