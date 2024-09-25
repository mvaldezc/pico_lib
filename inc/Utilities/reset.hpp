/***********************************************************************
 * @file	:	reset.hpp
 * @brief 	:	Reset utility
 *              Library to reset the microcontroller using the watchdog.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "hardware/watchdog.h"

namespace Utilities {

/**
 * @brief Reset the microcontroller using the watchdog timer.
 */
inline void reset()
{
    watchdog_enable(50, 1);
    while(1);
}

} // namespace Utilities
