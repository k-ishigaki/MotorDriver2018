#include "timer.hpp"
#include "../device/timer.hpp"
#include "hardware.hpp"

using namespace device::timer;
namespace ht = hardware::timer;

template<class T> class Timer_ : public Timer {
    using regs = timer_t<T::offset>;
    public:
        static Timer_<T>& getInstance(const ht::Config& config) {
            static Timer_<T> instance;
            instance.configure(config);
            return instance;
        }

        void configure(const ht::Config& config) override {
            uint8_t wfgMode = static_cast<uint8_t>(config.waveformGenerationMode);
            regs::TCCRA.WGM_0 = wfgMode & 0b0001;
            regs::TCCRA.WGM_1 = (wfgMode >> 1) & 0b0001;
            regs::TCCRB.WGM_2 = (wfgMode >> 2) & 0b0001;
            regs::TCCRB.WGM_3 = (wfgMode >> 3) & 0b0001;
            regs::TCCRB.ICNC = static_cast<uint8_t>(config.inputCaptureNoiseCanceler);
            regs::TCCRB.ICES = static_cast<uint8_t>(config.inputCaptureEdgeSelect);
            regs::TCCRB.CS = static_cast<uint8_t>(config.clock);
        }

        int16_t getCount() override {
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

Timer& ht::get1(const Config& config) { return Timer_<timer1_t>::getInstance(config); }
Timer& ht::get3(const Config& config) { return Timer_<timer3_t>::getInstance(config); }
Timer& ht::get4(const Config& config) { return Timer_<timer4_t>::getInstance(config); }
