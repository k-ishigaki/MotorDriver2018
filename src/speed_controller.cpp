#include "speed_controller.hpp"

#include "log.hpp"
#include <math.h>

namespace {
    constexpr int INTEGER_SCALE = 1000;
    constexpr int K_C1 = 0.210 * INTEGER_SCALE;
    constexpr int K_C2 = 3;
    constexpr int K_A1 = 0.019 * INTEGER_SCALE;
    constexpr int K_A2 = 3;
    constexpr int OFFSET = 7 * INTEGER_SCALE;

    constexpr float K_CP = 1.000;
    constexpr float K_CI = 0.400;
    constexpr float K_CD = 2.000;

    constexpr float K_AP = 1.000;
    constexpr float K_AI = 0.400;
    constexpr float K_AD = 2.000;
} /* namespace */

SpeedController::SpeedController(const Encoder& leftEncoder, const Encoder& rightEncoder,
                H_Bridge& leftBridge, H_Bridge& rightBridge)
        : leftEncoder(leftEncoder), rightEncoder(rightEncoder), leftBridge(leftBridge), rightBridge(rightBridge) {
            this->centerSpeedPID = new PIDController(K_CP, K_CI, K_CD);
            this->angularSpeedPID = new PIDController(K_AP, K_AI, K_AD);
            targetCenterSpeed = targetAngularSpeed = 0;
            lastTargetCenterSpeed = lastTargetAngularSpeed = 0;
        }

void SpeedController::setTarget(float centerSpeed, float angularSpeed) {
    lastTargetCenterSpeed = targetCenterSpeed;
    targetCenterSpeed = centerSpeed;
    lastTargetAngularSpeed = targetAngularSpeed;
    targetAngularSpeed = angularSpeed * 1000;
}

void SpeedController::determineOutput() {
    float leftSpeed = this->leftEncoder.getIntegratedLength() * 122;
    float rightSpeed = this->rightEncoder.getIntegratedLength() * 122;
    // 中心速度項の計算
    int measuredCenterSpeed = round((leftSpeed + rightSpeed) / 2);
    int targetCenterAcceleration =
        targetCenterSpeed - lastTargetCenterSpeed;
    int centerSpeedError = lastTargetCenterSpeed - measuredCenterSpeed;
    int centerTerm =
        K_C1 * ((long)targetCenterSpeed + K_C2 * targetCenterAcceleration +
                centerSpeedPID->calcCorrectionValue(centerSpeedError)) /
        INTEGER_SCALE;
    // 角速度項の計算
    int measuredAngularSpeed = round((rightSpeed - leftSpeed) * 1000 / 180.0);
    int targetAngularAcceleration =
        targetAngularSpeed - lastTargetAngularSpeed;
    int angularSpeedError = lastTargetAngularSpeed - measuredAngularSpeed;
    int angularTerm =
        K_A1 *
        ((long)targetAngularSpeed + K_A2 * targetAngularAcceleration +
         angularSpeedPID->calcCorrectionValue(angularSpeedError)) /
        INTEGER_SCALE;
    // 左右出力値計算
    int leftOutput = centerTerm - angularTerm + OFFSET / INTEGER_SCALE;
    int rightOutput = centerTerm + angularTerm + OFFSET / INTEGER_SCALE;
    // 出力
//    if (targetCenterSpeed == 0 && targetAngularSpeed == 0) {
//        leftBridge.changeDirection(H_Bridge::Direction::Stop);
//        rightBridge.changeDirection(H_Bridge::Direction::Stop);
//    } else {
    log_i("ct = %d, at = %d, mc = %d, ma = %d, l = %d, r = %d, ct = %d, at = %d", targetCenterSpeed, targetAngularSpeed, measuredCenterSpeed, measuredAngularSpeed, leftOutput, rightOutput, centerTerm, angularTerm);
    leftBridge.setDutyRatio(leftOutput > 0 ? leftOutput / 256.0 : 0);
    rightBridge.setDutyRatio(rightOutput > 0 ? rightOutput / 256.0 : 0);
//    }
}

