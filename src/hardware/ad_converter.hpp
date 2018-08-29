#ifndef AD_CONVERTER_H
#define AD_CONVERTER_H
#include <stdint.h>

class ADConverter {
    public:
        virtual void selectInputChannel() const = 0;
        virtual void startConversion() const = 0;
        virtual void isConverting() const = 0;
        virtual uint16_t getResult() const = 0;
};

#endif
