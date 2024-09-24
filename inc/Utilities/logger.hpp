#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "lock_guard.hpp"

namespace Utilities {

    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    // Function to log messages to uart/usb
    void log(LogLevel level, const char * message);

} // namespace Utilities
