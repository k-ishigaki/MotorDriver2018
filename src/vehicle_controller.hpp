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
	// 到達目標残中心距離(mm * ControlFreq)
	float remainingDistance;
	// 目標曲率[mm^(-1)]
	float targetCurvature;
	// 移動途中速度[mm/s], [rad/s]
	float movingCenterSpeed, movingAngularSpeed;
	// 最終目標速度[mm/s], [rad/s]
	float terminalCenterSpeed, terminalAngularSpeed;
	// 加速中目標速度[mm/s], 角速度[rad/s]
	float centerSpeed, angularSpeed;
};

#endif /* VEHICLE_CONTROLLER_H */
