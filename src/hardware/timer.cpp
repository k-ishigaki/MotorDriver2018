#include "timer.hpp"
#include "../device/timer.hpp"
#include "hardware.hpp"

using namespace device::timer;
namespace ht = hardware::timer;
namespace ht2 = hardware::timer2;

template<class T> class Timer_ : public Timer {
    using regs = timer_t<T::offset>;
    public:
        static Timer_<T>& getInstance(const ht::Config& config) {
            static Timer_<T> instance;
            uint8_t wfgMode = static_cast<uint8_t>(config.waveformGenerationMode);
            regs::TCCRA.WGM_0 = wfgMode & 0b0001;
            regs::TCCRA.WGM_1 = (wfgMode >> 1) & 0b0001;
            regs::TCCRB.WGM_2 = (wfgMode >> 2) & 0b0001;
            regs::TCCRB.WGM_3 = (wfgMode >> 3) & 0b0001;
            regs::TCCRB.ICNC = static_cast<uint8_t>(config.inputCaptureNoiseCanceler);
            regs::TCCRB.ICES = static_cast<uint8_t>(config.inputCaptureEdgeSelect);
            regs::TCCRB.CS = static_cast<uint8_t>(config.clock);
            return instance;
        }

        uint16_t getCount() override {
            auto f = []() { return regs::TCNT; };
            return hardware::noInterruptWithReturn(+f);
        }

        void setCount(uint16_t count) override {
            auto f = [](uint16_t count) { regs::TCNT = count; };
            hardware::noInterrupt(+f, count);
        }

        void start() override {
            // start is not implemented
        }

        void stop() override {
            // stop is not implemented
        }
};

class Timer2 : public Timer {
    using regs = timer2_t;
    public:
        static Timer2& getInstance(const ht2::Config& config) {
            static Timer2 instance;
            uint8_t wfgMode = static_cast<uint8_t>(config.waveformGenerationMode);
            regs::TCCRA.WGM_0 = wfgMode & 0b0001;
            regs::TCCRA.WGM_1 = (wfgMode >> 1) & 0b0001;
            regs::TCCRB.WGM_2 = (wfgMode >> 2) & 0b0001;
            regs::TCCRB.CS = static_cast<uint8_t>(config.clock);
            return instance;
        }

        uint16_t getCount() override {
            return regs::TCNT;
        }

        void setCount(uint16_t count) override {
            regs::TCNT = count;
        }

        void start() override {
            // start is not implemented
        }

        void stop() override {
            // stop is not implemented
        }
};

Timer& ht::get1(const Config& config) { return Timer_<timer1_t>::getInstance(config); }
Timer& ht::get3(const Config& config) { return Timer_<timer3_t>::getInstance(config); }
Timer& ht::get4(const Config& config) { return Timer_<timer4_t>::getInstance(config); }

Timer& ht2::get2(const Config& config) { return Timer2::getInstance(config); }

