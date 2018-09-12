#include "log.hpp"
#include <stdio.h>

using namespace application::log;

namespace {
    putcharImpl func = nullptr;
    Level level = Level::Error;
    int putImpl(char data, FILE* file) {
        (*func)(data);
        return 0;
    }
}

void application::log::configure(putcharImpl func, Level level) {
    ::func = func;
    ::level = level;
    fdevopen(::putImpl, nullptr);
}

bool application::log::impl::needsOutput(Level level) {
    return level >= ::level;
}
