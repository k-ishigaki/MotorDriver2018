#include "pwm.hpp"
#include "../device/timer.hpp"
#include "hardware.hpp"

using namespace device::timer;
namespace hp = hardware::pwm;

template<class T> class PWMA_ : public PWM {
    using regs = timer_t<T::offset>;
    public:
        static PWMA_<T>& getInstance(const hp::Config& config) {
            static PWMA_<T> instance;
            return instance;
        }

        void setOutputMode(hardware::pwm::OutputMode mode) override {
            regs::TCCRA.COMA = static_cast<uint8_t>(mode);
        }

        void setDutyCycle(uint16_t count) override {
            regs::OCRA = count;
        }
};

template<class T> class PWMB_ : public PWM {
    using regs = timer_t<T::offset>;
    public:
        static PWMB_<T>& getInstance(const hp::Config& config) {
            static PWMB_<T> instance;
            return instance;
        }

        void setOutputMode(hardware::pwm::OutputMode mode) override {
            regs::TCCRA.COMB = static_cast<uint8_t>(mode);
        }

        void setDutyCycle(uint16_t count) override {
            regs::OCRB = count;
        }
};

PWM& hp::get1A(const Config& config) { return PWMA_<timer1_t>::getInstance(config); }
PWM& hp::get1B(const Config& config) { return PWMB_<timer1_t>::getInstance(config); }
PWM& hp::get3A(const Config& config) { return PWMA_<timer3_t>::getInstance(config); }
PWM& hp::get3B(const Config& config) { return PWMB_<timer3_t>::getInstance(config); }
PWM& hp::get4A(const Config& config) { return PWMA_<timer4_t>::getInstance(config); }
PWM& hp::get4B(const Config& config) { return PWMB_<timer4_t>::getInstance(config); }
