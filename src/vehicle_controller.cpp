#include "vehicle_controller.hpp"

#include "config.hpp"
#include "log.hpp"
#include <stdlib.h>
#include <math.h>

namespace {
    constexpr long MAX_CENTER_ACCELERATION = 600;
    constexpr long MAX_ANGULAR_ACCELERATION = 1.5 * M_PI * 1000;
    constexpr int CONTROL_FREQ = config::ControlFreq;
    constexpr int MAX_CENTER_SPEED_INC = MAX_CENTER_ACCELERATION / CONTROL_FREQ;
    constexpr int MAX_ANGULAR_SPEED_INC = MAX_ANGULAR_ACCELERATION / CONTROL_FREQ;
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
	this->targetCurvature = round(curvature * 1000000);
	this->movingCenterSpeed = round(movingCenterSpeed);
	this->movingAngularSpeed = round(movingCenterSpeed * curvature * 1000);
	this->terminalCenterSpeed = round(terminalCenterSpeed);
	this->terminalAngularSpeed =
	    round(terminalCenterSpeed * curvature * 1000);
	this->remainingDistance = round(distance * CONTROL_FREQ * 1.01);
}

void VehicleController::determineOutput() {
    if (this->remainingDistance <= 0) {
		this->speedCo.setTarget(this->terminalCenterSpeed,
                static_cast<float>(this->terminalAngularSpeed) / 1000);
		this->speedCo.determineOutput();
        return;
    }
	// 目標中心加速度
	int targetCenterAcceleration;
	// 目標角加速度
	int targetAngularAcceleration;

	// 前回中心速度を残距離から引く
	this->remainingDistance -= centerSpeed;
	if (this->remainingDistance < 0) {
		this->remainingDistance = 0;
		this->speedCo.setTarget(this->terminalCenterSpeed,
                static_cast<float>(this->terminalAngularSpeed) / 1000);
		this->speedCo.determineOutput();
        return;
	}

	// 目標中心加速度を求める
	int accelerationTime = ((long)abs(this->terminalCenterSpeed - this->centerSpeed)) *
	                       1000 / MAX_CENTER_ACCELERATION;
	long accelerationDistance = (long)accelerationTime * CONTROL_FREQ *
	                            (this->centerSpeed + this->terminalCenterSpeed) /
	                            (2 * 1000);
	if (this->remainingDistance < accelerationDistance) {
		// 最大加速度で最終目標速度に向けて加速する
		int diff = this->terminalCenterSpeed - this->centerSpeed;
		if (abs(diff) <= MAX_CENTER_SPEED_INC) {
			targetCenterAcceleration = diff;
		} else {
			targetCenterAcceleration = diff >= 0
			                               ? MAX_CENTER_SPEED_INC
			                               : -MAX_CENTER_SPEED_INC;
		}
	} else if (labs(this->remainingDistance - accelerationDistance) <
	           this->centerSpeed * 2) {
		// 最終目標速度に向けて加速する
		targetCenterAcceleration =
		    (long)(this->terminalCenterSpeed - this->centerSpeed) *
		    (this->terminalCenterSpeed + this->centerSpeed) /
		    (2 * this->remainingDistance);
	} else {
		// 最大加速度で移動中目標速度に向けて加速する
		int diff = this->movingCenterSpeed - this->centerSpeed;
		if (abs(diff) <= MAX_CENTER_SPEED_INC) {
			targetCenterAcceleration = diff;
		} else {
			targetCenterAcceleration = diff >= 0
			                               ? MAX_CENTER_SPEED_INC
			                               : -MAX_CENTER_SPEED_INC;
		}
	}

	// 目標曲率に合わせて目標中心加速度を修正し、目標各加速度を求める
	int nextMaxAngularSpeed = this->angularSpeed + MAX_ANGULAR_SPEED_INC;
	int nextMinAngularSpeed = this->angularSpeed - MAX_ANGULAR_SPEED_INC;
	int nextMinCenterSpeed = this->centerSpeed - MAX_CENTER_SPEED_INC;
	int nextRequiredCenterSpeed = this->centerSpeed + targetCenterAcceleration;
	if (nextMinCenterSpeed > 0 &&
	    this->targetCurvature <=
	        nextMinAngularSpeed * 1000L / nextMinCenterSpeed) {
		// 最大曲率変動（負）でも目標曲率に届かない場合は、その最大曲率
		// 変動に合わせて目標中心加速度、目標角加速度を求める
		targetCenterAcceleration = -MAX_CENTER_SPEED_INC;
		targetAngularAcceleration = -MAX_ANGULAR_SPEED_INC;
	} else if (nextMinCenterSpeed > 0 &&
	           this->targetCurvature >=
	               nextMaxAngularSpeed * 1000L / nextMinCenterSpeed) {
		// 最大曲率変動（正）でも目標曲率に届かない場合は、その最大曲率
		// 変動に合わせて目標中心加速度、目標角加速度を求める
		targetCenterAcceleration = -MAX_CENTER_SPEED_INC;
		targetAngularAcceleration = MAX_ANGULAR_SPEED_INC;
	} else if (nextRequiredCenterSpeed == 0) {
        // 次の要求速度が0の場合は、角加速度も0とする
        targetAngularAcceleration = 0;
    } else if (this->targetCurvature <=
	           nextMinAngularSpeed * 1000L / nextRequiredCenterSpeed) {
		// 要求中心加速度では目標曲率に追従できない場合は、最大角加速度
		// （負）に合わせて目標中心加速度を求める
		targetCenterAcceleration =
		    nextMinAngularSpeed * 1000L / this->targetCurvature - this->centerSpeed;
		targetAngularAcceleration = -MAX_ANGULAR_SPEED_INC;
	} else if (this->targetCurvature >=
	           nextMaxAngularSpeed * 1000L / nextRequiredCenterSpeed) {
		// 要求中心加速度では目標曲率に追従できない場合は、最大角加速度
		// （正）に合わせて目標中心加速度を求める
		targetCenterAcceleration =
		    nextMaxAngularSpeed * 1000L / this->targetCurvature - this->centerSpeed;
		targetAngularAcceleration = MAX_ANGULAR_SPEED_INC;
	} else {
		// 要求中心加速度で目標曲率に追従できる場合は、角加速度を要求中
		// 心加速度から求める
        targetAngularAcceleration =
            nextRequiredCenterSpeed * this->targetCurvature / 1000L -
            this->angularSpeed;
	}
	// それぞれの加速度を適用
	this->centerSpeed += targetCenterAcceleration;
	this->angularSpeed += targetAngularAcceleration;

	this->speedCo.setTarget(this->centerSpeed, static_cast<float>(this->angularSpeed) / 1000);
	this->speedCo.determineOutput();
}
