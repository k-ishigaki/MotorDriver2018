#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>


namespace device {
    using address_t = uint16_t;

    template<typename T> inline T writeToReg(address_t addr, T data) {
        return *reinterpret_cast<volatile T*>(addr) = data;
    }

    template<typename T> inline T readFromReg(address_t addr) {
        return *reinterpret_cast<volatile T*>(addr);
    }

    template<address_t addr, typename T> struct register_t {
        using value_type = T;

        static value_type write(value_type data) {
            return writeToReg(addr, data);
        }

        static value_type read() {
            return readFromReg<value_type>(addr);
        }

        value_type operator = (value_type data) { return write(data); }

        operator value_type() { return read(); }

        value_type operator |= (value_type data) { return write(read() | data); }

        value_type operator &= (value_type data) { return write(read() & data); }
    };


    enum class Bitpos : uint8_t {
        B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
    };

    template<class T, Bitpos pos, uint8_t len> struct bits_t {
        typename T::value_type get() {
            return (T::read() >> static_cast<typename T::value_type>(pos))
                & ((1 << len) - 1);
        }

        void set(typename T::value_type v) {
            auto m = static_cast<typename T::value_type>(
                    ((1 << len) - 1) << static_cast<typename T::value_type>(pos)
                    );
            T::write((T::read() & ~m)
                    | (static_cast<typename T::value_type>(v)
                        << static_cast<typename T::value_type>(pos)));
        }

        void operator = (typename T::value_type v) { set(v); }

        operator typename T::value_type() { return get(); }
    };
}

#endif
