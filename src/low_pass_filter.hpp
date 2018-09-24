#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

class LowPassFilter {
    public:
        LowPassFilter(float cutOffFreq, float q);
        float apply(float input);

    private:
        const float a1, a2, b0, b1;
        float out1, out2;
        float in1, in2;
};

#endif
