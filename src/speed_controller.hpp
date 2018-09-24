#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

#include "encoder.hpp"
#include "h_bridge.hpp"
#include "pid_controller.hpp"

class SpeedController {
    public:
        SpeedController(const Encoder& leftEncoder, const Encoder& rightEncoder,
                H_Bridge& leftBridge, H_Bridge& rightBridge);

        /**
         * Set target speed.
         *
         * @param center speed [mm/s]
         * @param angular speed [rad/s]
         */
        void setTarget(float centerSpeed, float angularSpeed);
        void determineOutput(void);

    private:
        const Encoder& leftEncoder;
        const Encoder& rightEncoder;
        H_Bridge& leftBridge;
        H_Bridge& rightBridge;
        PIDController* centerSpeedPID;
        PIDController* angularSpeedPID;
        // 目標速度
        int targetCenterSpeed;
        int targetAngularSpeed;
        // 前回目標速度
        int lastTargetCenterSpeed;
        int lastTargetAngularSpeed;
};

#endif
