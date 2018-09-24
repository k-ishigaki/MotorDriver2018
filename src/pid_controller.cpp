#include "pid_controller.hpp"

namespace {
    const float CUT_OFF_FREQ = 5.0;
    const float Q = 0.707; // 1 / root(2)
} /* namespace */

// TODO 微分時間と積分時間も決められるようにする
PIDController::PIDController(float K_P, float K_I, float K_D)
    : K_P(K_P), K_I(K_I), K_D(K_D), integral(0), lastDeviation(0),
    filter(CUT_OFF_FREQ, Q) {
    }

PIDController::~PIDController() {}

float PIDController::calcCorrectionValue(float deviation) {
    // 比例項
    float proportional = deviation;
    // 積分項
    integral += deviation;
    // 微分項
    float differential = filter.apply(deviation - lastDeviation);
    lastDeviation = deviation;

    float result = 0.0;
    result += K_P * proportional;
    result += K_I * integral;
    result += K_D * differential;
    return result;
}
