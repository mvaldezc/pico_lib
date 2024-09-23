#pragma once
#include <mutex>

using mutex_t = std::mutex;

// instantiate a mutex from its name
#define auto_init_mutex(name) static mutex_t name

inline void mutex_init(mutex_t * mtx) {
    // do nothing
}
