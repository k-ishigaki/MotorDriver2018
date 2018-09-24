#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "low_pass_filter.hpp"

class PIDController {
    public:
        PIDController(float K_P, float K_I, float K_D);
        virtual ~PIDController();
        float calcCorrectionValue(float deviation);

    private:
        // 各項の係数
        const float K_P, K_I, K_D;
        // 積分項用積算値
        float integral;
        // 微分項用前回偏差値
        float lastDeviation;
        // 実用微分のためのローパスフィルター
        LowPassFilter filter;
};

#endif
