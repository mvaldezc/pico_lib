#pragma once
#include "hardware/watchdog.h"

namespace Utilities {

// Reset the microcontroller with watchdog
inline void reset()
{
    watchdog_enable(50, 1);
    while(1);
}

} // namespace Utilities
