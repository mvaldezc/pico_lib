#include "reset.hpp"

namespace Utilities {
    bool resetCalled = false;
    void reset()
    {
        resetCalled = true;
    }
} // namespace Utilities