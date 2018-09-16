#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include "framework/digital_output_pin.hpp"
#include "hardware/interrupt.hpp"
#include "hardware/pwm.hpp"
#include "hardware/timer.hpp"

namespace ht = hardware::timer;
namespace hp = hardware::pwm;

class H_Bridge {
    public:
        /**
         * Construct H-Bridge.
         *
         * @param pwm(compare match) interface for high side pin 0
         * @param pwm(compare match) interface for low side pin 0
         * @param pwm output mode which enable pwm output
         * @param pwm output mode which disable pwm output
         * @param digital output pin interface for low side pin 0
         * @param digital output pin interface for high side pin 0
         * @param digital output pin interface for low side pin 1
         * @param digital output pin interface for high side pin 1
         * @param compare match interrupt interface<br>
         *        which can be used while high side pin 0 pwm is disabled
         * @param compare match interrupt interface<br>
         *        which can be used while high side pin 1 pwm is disabled
         * @param pwm(compare match) interface for compare match
         * @param compare match interrupt interface<br>
         *        かけないので日本語で書く
         *        このコンペアマッチ割り込みのソースのタイマーは他のPWM/CompareMatchの
         *        割り込みのソースのタイマーと同じカウントでないといけない
         *        かつ、常に使用可能である必要がある
         * @param timer overflow interrupt interface whose timer is base of pwm
         */
        H_Bridge(
                PWM&,
                PWM&,
                hp::OutputMode,
                hp::OutputMode,
                DigitalOutputPin&,
                DigitalOutputPin&,
                DigitalOutputPin&,
                DigitalOutputPin&,
                Interrupt&,
                Interrupt&,
                PWM&,
                Interrupt&,
                Interrupt&);

        enum class Direction {
            Free,
            Stop,
            Forward,
            Backward,
        };

        /**
         * Change direction of h-bridge current.
         *
         * @param direction of h-bridge
         */
        void changeDirection(Direction);

        /**
         * Set duty ratio
         *
         * The actual duty cycle count may be limited due to hardware structure.
         *
         * @param ratio between 0.0 ~ 1.0
         */
        void setDutyRatio(float);

    private:
        PWM& pwm_0h;
        PWM& pwm_1h;
        hp::OutputMode enabledMode;
        hp::OutputMode disabledMode;
        DigitalOutputPin& pin_0l;
        DigitalOutputPin& pin_0h;
        DigitalOutputPin& pin_1l;
        DigitalOutputPin& pin_1h;
        Interrupt& compareMatchInterrupt0;
        Interrupt& compareMatchInterrupt1;
        PWM& pwm_l;
        Interrupt& compareMatchInterrupt2;
        Interrupt& timerOverflowInterrupt;

        Direction currentDirection;
        class CompareMatchInterruptHandler0 : public InterruptHandler {
            public:
                CompareMatchInterruptHandler0(H_Bridge* outer);
                void handleInterrupt() override;
            private:
                H_Bridge* outer;
        };
        class CompareMatchInterruptHandler1 : public InterruptHandler {
            public:
                CompareMatchInterruptHandler1(H_Bridge* outer);
                void handleInterrupt() override;
            private:
                H_Bridge* outer;
        };
        class CompareMatchInterruptHandler2 : public InterruptHandler {
            public:
                CompareMatchInterruptHandler2(H_Bridge* outer);
                void handleInterrupt() override;
            private:
                H_Bridge* outer;
        };
        class TimerOverflowInterruptHandler : public InterruptHandler {
            public:
                TimerOverflowInterruptHandler(H_Bridge* outer);
                void handleInterrupt() override;
            private:
                H_Bridge* outer;
        };
};

#endif
