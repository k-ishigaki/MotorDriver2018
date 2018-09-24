#ifndef ENCODER_H
#define ENCODER_H

#include "hardware/interrupt.hpp"
#include "framework/digital_output_pin.hpp"

class Encoder {
    public:
        /**
         * Construct encoder.
         *
         * @param interrupt instance for count source
         * @param led pin for debug
         */
        Encoder(Interrupt&, DigitalOutputPin&);

        /**
         * Load integrated count and calc length.
         *
         * A periodic call of this method must be applied to calculate integrated
         * length for each control loop.
         */
        void load();

        /**
         * Get integrated count at last load.
         *
         * @return integrated length
         */
        float getIntegratedLength() const;

    private:
        DigitalOutputPin& led;
        uint8_t count;
        float integratedLength;
        class CountInterruptHandler : public InterruptHandler {
            public:
                CountInterruptHandler(Encoder* outer);
                void handleInterrupt() override;
            private:
                Encoder* outer;
        };
};

#endif
