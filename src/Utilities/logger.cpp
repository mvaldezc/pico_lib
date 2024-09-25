#include "logger.hpp"

namespace Utilities {

    mutex_t logMutex;

    void log(LogLevel level, const char * message)
    {
        // If Release build, do not print DEBUG logs
        #ifdef NDEBUG
            if (level == LogLevel::DEBUG)
            {
                return;
            }
        #endif

        // If not initialized, initialize stdio
        static bool initialized = false;
        if (!initialized)
        {
            stdio_init_all();
            initialized = true;
        }

        // Lock mutex to prevent multiple threads from writing 
        // to stdout at the same time.
        lock_guard<mutex_t> lock(logMutex);

        switch (level)
        {
            case LogLevel::DEBUG:
                printf("[DEBUG]: %s\n", message);
                break;
            case LogLevel::INFO:
                printf("[INFO]: %s\n", message);
                break;
            case LogLevel::WARNING:
                printf("[WARNING]: %s\n", message);
                break;
            case LogLevel::ERROR:
                printf("[ERROR]: %s\n", message);
                break;
            default:
                break;
        }
    }

} // namespace Utilities
