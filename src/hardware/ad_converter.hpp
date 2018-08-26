#ifndef AD_CONVERTER_H
#define AD_CONVERTER_H
#include <stdint.h>

class ADConverter {
    public:
        virtual void selectInputChannel(void) const = 0;
        virtual void startConversion(void) const = 0;
        virtual void isConverting(void) const = 0;
        virtual uint16_t getResult(void) const = 0;
};

#endif
