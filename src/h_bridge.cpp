#include "h_bridge.hpp"
#include "log.hpp"

H_Bridge::H_Bridge(
        PWM& pwm_0h,
        PWM& pwm_1h,
        hp::OutputMode enabledMode,
        hp::OutputMode disabledMode,
        DigitalOutputPin& pin_0l,
        DigitalOutputPin& pin_0h,
        DigitalOutputPin& pin_1l,
        DigitalOutputPin& pin_1h,
        Interrupt& compareMatchInterrupt0,
        Interrupt& compareMatchInterrupt1,
        PWM& pwm_l,
        Interrupt& compareMatchInterrupt2,
        Interrupt& timerOverflowInterrupt) :
    pwm_0h(pwm_0h), pwm_1h(pwm_1h),
    enabledMode(enabledMode), disabledMode(disabledMode),
    pin_0l(pin_0l), pin_0h(pin_0h), pin_1l(pin_1l), pin_1h(pin_1h),
    compareMatchInterrupt0(compareMatchInterrupt0),
    compareMatchInterrupt1(compareMatchInterrupt1),
    pwm_l(pwm_l),
    compareMatchInterrupt2(compareMatchInterrupt2),
    timerOverflowInterrupt(timerOverflowInterrupt) {
        this->pwm_0h.setOutputMode(disabledMode);
        this->pwm_1h.setOutputMode(disabledMode);
        this->pin_0h.write(false);
        this->pin_0l.write(false);
        this->pin_1h.write(false);
        this->pin_1l.write(false);
        this->compareMatchInterrupt0.registerHandler(
                new CompareMatchInterruptHandler0(this));
        this->compareMatchInterrupt1.registerHandler(
                new CompareMatchInterruptHandler1(this));
        this->pwm_l.setOutputMode(disabledMode);
        this->pwm_l.setDutyCycle(1023 - 30);
        this->compareMatchInterrupt2.registerHandler(
                new CompareMatchInterruptHandler2(this));
        this->timerOverflowInterrupt.registerHandler(
                new TimerOverflowInterruptHandler(this));
        this->currentDirection = Direction::Free;
    }

void H_Bridge::changeDirection(Direction direction) {
    if (this->currentDirection == direction) {
        log_i("set to same direction, ignored");
        return;
    }
    switch (direction) {
        case Direction::Free:
            this->compareMatchInterrupt0.disable();
            this->compareMatchInterrupt1.disable();
            this->compareMatchInterrupt2.disable();
            this->pwm_0h.setOutputMode(disabledMode);
            this->pwm_1h.setOutputMode(disabledMode);
            this->pin_0h.write(false);
            this->pin_0l.write(false);
            this->pin_1h.write(false);
            this->pin_1l.write(false);
            log_i("Free");
            break;
        case Direction::Stop:
            this->compareMatchInterrupt0.disable();
            this->compareMatchInterrupt1.disable();
            this->compareMatchInterrupt2.disable();
            this->pwm_0h.setOutputMode(disabledMode);
            this->pwm_1h.setOutputMode(disabledMode);
            this->pin_0h.write(false);
            this->pin_0l.write(true);
            this->pin_1h.write(false);
            this->pin_1l.write(true);
            log_i("Stop");
            break;
        case Direction::Forward:
            this->compareMatchInterrupt0.disable();
            this->pwm_0h.setOutputMode(enabledMode);
            this->pwm_1h.setOutputMode(disabledMode);
            this->pin_0h.write(true); // workaround for OC3B and OC4B, see https://www.avrfreaks.net/forum/atmega328pb-timer-34-output-compare-pwm-issue
            this->pin_0l.write(false);
            this->pin_1h.write(false);
            this->pin_1l.write(true);
            this->compareMatchInterrupt1.enable();
            this->compareMatchInterrupt2.enable();
            log_i("Forward");
            break;
        case Direction::Backward:
            this->compareMatchInterrupt1.disable();
            this->pwm_0h.setOutputMode(disabledMode);
            this->pwm_1h.setOutputMode(enabledMode);
            this->pin_0h.write(false);
            this->pin_0l.write(true);
            this->pin_1h.write(true); // workaround for OC3B and OC4B, see https://www.avrfreaks.net/forum/atmega328pb-timer-34-output-compare-pwm-issue
            this->pin_1l.write(false);
            this->compareMatchInterrupt0.enable();
            this->compareMatchInterrupt2.enable();
            log_i("Backward");
            break;
    }
    this->currentDirection = direction;
}

void H_Bridge::setDutyRatio(float ratio) {
    uint16_t count = 1023 * ratio;
    count = count > 900 ? 900 : count;
    count = count < 100 ? 100 : count;
    switch (this->currentDirection) {
        case Direction::Forward:
            this->pwm_0h.setDutyCycle(count);
            this->pwm_1h.setDutyCycle(count + 20);
            break;
        case Direction::Backward:
            this->pwm_1h.setDutyCycle(count);
            this->pwm_0h.setDutyCycle(count + 20);
            break;
        default:
            log_i("mode is not pwm, ignored");
            break;
    }
}

H_Bridge::CompareMatchInterruptHandler0::CompareMatchInterruptHandler0(H_Bridge* outer) {
    this->outer = outer;
}

void H_Bridge::CompareMatchInterruptHandler0::handleInterrupt() {
    if (this->outer->currentDirection != H_Bridge::Direction::Backward) {
        log_e("illegal interrupt dir=%d",
                static_cast<uint8_t>(this->outer->currentDirection));
        return;
    }
    this->outer->pin_1l.write(true);
}

H_Bridge::CompareMatchInterruptHandler1::CompareMatchInterruptHandler1(H_Bridge* outer) {
    this->outer = outer;
}

void H_Bridge::CompareMatchInterruptHandler1::handleInterrupt() {
    if (this->outer->currentDirection != H_Bridge::Direction::Forward) {
        log_e("illegal interrupt dir=%d",
                static_cast<uint8_t>(this->outer->currentDirection));
        return;
    }
    this->outer->pin_0l.write(true);
}

H_Bridge::CompareMatchInterruptHandler2::CompareMatchInterruptHandler2(H_Bridge* outer) {
    this->outer = outer;
}

void H_Bridge::CompareMatchInterruptHandler2::handleInterrupt() {
    switch (this->outer->currentDirection) {
        case H_Bridge::Direction::Forward:
            this->outer->pin_0l.write(false);
            break;
        case H_Bridge::Direction::Backward:
            this->outer->pin_1l.write(false);
            break;
        default:
            log_e("illegal interrupt dir=%d",
                    static_cast<uint8_t>(this->outer->currentDirection));
            break;
    }
}

H_Bridge::TimerOverflowInterruptHandler::TimerOverflowInterruptHandler(H_Bridge* outer) {
    this->outer = outer;
}

void H_Bridge::TimerOverflowInterruptHandler::handleInterrupt() {
}
