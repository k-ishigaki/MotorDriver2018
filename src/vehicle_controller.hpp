#ifndef VEHICLE_CONTROLLER_H
#define VEHICLE_CONTROLLER_H

#include "speed_controller.hpp"

class VehicleController {
public:
	VehicleController(SpeedController& speedCo);
	void setTarget(float curvature, float distance, float movingCenterSpeed,
	               float terminalCenterSpeed);
	void determineOutput(void);

private:
	SpeedController& speedCo;
	// 到達目標残中心距離
	long remainingDistance;
	// 目標曲率[10^6mm^(-1)]
	long targetCurvature;
	// 移動途中速度[mm/s], [10^3rad/s]
	int movingCenterSpeed, movingAngularSpeed;
	// 最終目標速度[mm/s], [10^3rad/s]
	int terminalCenterSpeed, terminalAngularSpeed;
	// 加速中目標速度[mm/s], 角速度[10^3rad/s]
	int centerSpeed, angularSpeed;
};

#endif /* VEHICLE_CONTROLLER_H */
