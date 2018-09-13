#include "log.hpp"
#include <stdio.h>

using namespace application::log;

namespace {
    void (*putCharImpl)(char) = nullptr;
    Level level = Level::Error;
    int putImpl(char data, FILE* file) {
        (*putCharImpl)(data);
        return 0;
    }
}

void application::log::configure(void (*putCharImpl)(char), Level level) {
    ::putCharImpl = putCharImpl;
    ::level = level;
    fdevopen(::putImpl, nullptr);
}

bool application::log::impl::needsOutput(Level level) {
    return level >= ::level;
}
