#pragma once
#include <mutex>

using critical_section_t = std::mutex;

inline void critical_section_init(void *cs)
{
    // do nothing
}

inline void critical_section_enter_blocking(critical_section_t *cs)
{
    cs->lock();
}

inline void critical_section_exit(critical_section_t *cs)
{
    cs->unlock();
}

inline void critical_section_deinit(void *cs)
{
    // do nothing
}
