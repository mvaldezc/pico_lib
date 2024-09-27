/***********************************************************************
 * @file	:	logger.hpp
 * @brief 	:	Logger utility
 *              Library to log messages to stdout with different levels.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "lock_guard.hpp"

namespace Utilities {

    /**
     * @brief Enum class to define the logger levels.
     */
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    /**
     * @brief Function to log messages to stdout.
     * @param[in] level LogLevel enum class.
     * @param[in] message Message to log.
     */
    void log(LogLevel level, const char * message);

} // namespace Utilities
