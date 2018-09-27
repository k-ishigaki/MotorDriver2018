#ifndef ENCODER_H
#define ENCODER_H

#include "hardware/interrupt.hpp"
#include "framework/digital_output_pin.hpp"
#include "framework/digital_input_pin.hpp"

class Encoder {
    public:
        /**
         * Construct encoder.
         *
         * @param interrupt instance for count source (assumes pin change interrupt)
         * @param encoder input pin (used to check duplicate count)
         * @param led pin for debug
         */
        Encoder(Interrupt&, DigitalInputPin&, DigitalOutputPin&);

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
        uint8_t count;
        float integratedLength;
        class CountInterruptHandler : public InterruptHandler {
            public:
                CountInterruptHandler(Encoder*, DigitalInputPin&, DigitalOutputPin&);
                void handleInterrupt() override;
            private:
                Encoder* outer;
                DigitalInputPin& inputPin;
                DigitalOutputPin& led;
                bool currentState;
        };
};

#endif
