#include "vehicle_controller.hpp"

#include "config.hpp"
#include <stdlib.h>
#include <math.h>

namespace {
    constexpr float MAX_CENTER_ACCELERATION = 600;
    constexpr float MAX_ANGULAR_ACCELERATION = 2 * M_PI;
    constexpr float MAX_CENTER_SPEED_INC = MAX_CENTER_ACCELERATION / config::ControlFreq;
    constexpr float MAX_ANGULAR_SPEED_INC = MAX_ANGULAR_ACCELERATION / config::ControlFreq;
    constexpr float MIN_CURVE_RADIUS = config::WheelTread / 2;
} /* namespace */

VehicleController::VehicleController(SpeedController& speedCo)
    : speedCo(speedCo) {
        this->remainingDistance = 0;
        this->movingCenterSpeed = this->movingAngularSpeed = 0;
        this->terminalCenterSpeed = this->terminalAngularSpeed = 0;
        this->centerSpeed = this->angularSpeed = 0;
    }

void VehicleController::setTarget(float curvature, float distance,
        float movingCenterSpeed,
        float terminalCenterSpeed) {
    // 最小移動半径による曲率の制限
    if (fabs(curvature) > 1.0 / MIN_CURVE_RADIUS) {
        curvature = curvature >= 0 ? 1.0 / MIN_CURVE_RADIUS
            : -1.0 / MIN_CURVE_RADIUS;
    }
    this->targetCurvature = curvature;
    this->movingCenterSpeed = movingCenterSpeed;
    this->movingAngularSpeed = movingCenterSpeed * curvature;
    this->terminalCenterSpeed = terminalCenterSpeed;
    this->terminalAngularSpeed = terminalCenterSpeed * curvature;
    this->remainingDistance = distance * config::ControlFreq;
}

void VehicleController::determineOutput() {
    // 目標中心加速度
    float targetCenterSpeedInc;
    // 目標角加速度
    float targetAngularSpeedInc;

    // 前回中心速度を残距離から引く
    this->remainingDistance -= centerSpeed;
    if (this->remainingDistance < 0) {
        this->remainingDistance = 0;
    }

    // 目標中心加速度を求める
    auto accelerationTime = fabs(this->terminalCenterSpeed - this->centerSpeed) / MAX_CENTER_ACCELERATION;
    auto accelerationDistance = accelerationTime * config::ControlFreq *
        (this->centerSpeed + this->terminalCenterSpeed) / 2;
    if (this->remainingDistance < accelerationDistance + this->centerSpeed) {
        // 最終目標速度に向けて加速する
        targetCenterSpeedInc =
            (this->terminalCenterSpeed - this->centerSpeed) *
            (this->terminalCenterSpeed + this->centerSpeed) /
            (2 * this->remainingDistance);
    } else {
        // 最大加速度で移動中目標速度に向けて加速する
        targetCenterSpeedInc = this->movingCenterSpeed - this->centerSpeed;
    }
    targetCenterSpeedInc =
        targetCenterSpeedInc < -MAX_CENTER_SPEED_INC ? -MAX_CENTER_SPEED_INC
        : targetCenterSpeedInc > MAX_CENTER_SPEED_INC ? MAX_CENTER_SPEED_INC
        : targetCenterSpeedInc;

    // 目標曲率に合わせて目標中心加速度を修正し、目標各加速度を求める
    auto nextMaxAngularSpeed = this->angularSpeed + MAX_ANGULAR_SPEED_INC;
    auto nextMinAngularSpeed = this->angularSpeed - MAX_ANGULAR_SPEED_INC;
    auto nextMinCenterSpeed = this->centerSpeed - MAX_CENTER_SPEED_INC;
    nextMinCenterSpeed = nextMinCenterSpeed < 0 ? 0 : nextMinCenterSpeed;
    auto nextMaxCenterSpeed = this->centerSpeed + MAX_CENTER_SPEED_INC;
    auto nextRequiredCenterSpeed = this->centerSpeed + targetCenterSpeedInc;

    auto nextCenterSpeedWhenMaxCurvature = nextMaxAngularSpeed > 0
        ? nextMinCenterSpeed : nextMaxCenterSpeed;
    auto nextCenterSpeedWhenMinCurvature = nextMinAngularSpeed > 0
        ? nextMaxCenterSpeed : nextMinCenterSpeed;

    if (this->targetCurvature * nextCenterSpeedWhenMinCurvature < nextMinAngularSpeed) {
        // 最大曲率変動（負）でも目標曲率に届かない場合は、その最大曲率
        // 変動に合わせて目標中心加速度、目標角加速度を求める
        targetCenterSpeedInc = nextMinCenterSpeed - this->centerSpeed;
        targetAngularSpeedInc = -MAX_ANGULAR_SPEED_INC;
    } else if (this->targetCurvature * nextCenterSpeedWhenMaxCurvature > nextMaxAngularSpeed) {
        // 最大曲率変動（正）でも目標曲率に届かない場合は、その最大曲率
        // 変動に合わせて目標中心加速度、目標角加速度を求める
        targetCenterSpeedInc = nextMinCenterSpeed - this->centerSpeed;
        targetAngularSpeedInc = MAX_ANGULAR_SPEED_INC;
    } else if (this->targetCurvature * nextRequiredCenterSpeed < nextMinAngularSpeed) {
        // 要求中心加速度では目標曲率に追従できない場合は、
        // 目標曲率となるように要求中心加速度を修正する
        targetCenterSpeedInc = nextMinAngularSpeed / this->targetCurvature - this->centerSpeed;
        targetAngularSpeedInc = -MAX_ANGULAR_SPEED_INC;
    } else if (this->targetCurvature * nextRequiredCenterSpeed > nextMaxAngularSpeed) {
        // 要求中心加速度では目標曲率に追従できない場合は、
        // 目標曲率となるように要求中心加速度を修正する
        targetCenterSpeedInc = nextMaxAngularSpeed / this->targetCurvature - this->centerSpeed;
        targetAngularSpeedInc = MAX_ANGULAR_SPEED_INC;
    } else {
        // 要求中心加速度で目標曲率に追従できる場合は、
        // 角加速度を要求中心加速度から求める
        targetAngularSpeedInc = nextRequiredCenterSpeed * this->targetCurvature - this->angularSpeed;
    }
    targetCenterSpeedInc =
        targetCenterSpeedInc < -MAX_CENTER_SPEED_INC ? -MAX_CENTER_SPEED_INC
        : targetCenterSpeedInc > MAX_CENTER_SPEED_INC ? MAX_CENTER_SPEED_INC
        : targetCenterSpeedInc;

    // それぞれの加速度を適用
    this->centerSpeed += targetCenterSpeedInc;
    this->centerSpeed = this->centerSpeed > 0 ? this->centerSpeed : 0;
    this->angularSpeed += targetAngularSpeedInc;

    this->speedCo.setTarget(this->centerSpeed, this->angularSpeed);
    this->speedCo.determineOutput();
}
