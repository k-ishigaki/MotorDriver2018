#include "low_pass_filter.hpp"

#include <math.h>

// control freq = 12Hhz
#define OMEGA (2.0 * M_PI * cutOffFreq / 122)
#define ALPHA (sin(OMEGA) / (2.0 * q))
#define A0 (1.0 + ALPHA)

LowPassFilter::LowPassFilter(float cutOffFreq, float q)
    : a1((-2.0 * cos(OMEGA)) / A0), a2((1.0 - ALPHA) / A0),
    b0((1.0 - cos(OMEGA)) / 2.0 / A0), b1((1.0 - cos(OMEGA)) / A0) {
        out1 = out2 = 0.0;
        in1 = in2 = 0.0;
    }

float LowPassFilter::apply(float input) {
    float output = b0 * input + b1 * in1 + b0 * in2 - a1 * out1 - a2 * out2;
    in2 = in1;
    in1 = input;
    out2 = out1;
    out1 = output;
    return output;
}

