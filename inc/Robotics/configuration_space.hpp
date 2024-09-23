#pragma once

namespace Robotics {

    // Helper macros to generate the fields
    #define FIELD(N) float q##N;
    #define REPEAT_1(F) F(1)
    #define REPEAT_2(F) F(2) REPEAT_1(F)
    #define REPEAT_3(F) F(3) REPEAT_2(F)
    #define REPEAT_4(F) F(4) REPEAT_3(F)
    #define REPEAT_5(F) F(5) REPEAT_4(F)
    #define REPEAT_6(F) F(6) REPEAT_5(F)
    #define REPEAT_7(F) F(7) REPEAT_6(F)
    #define REPEAT_8(F) F(8) REPEAT_7(F)

    #define DEFINE_WAYPOINT(N) \
            typedef struct { \
                REPEAT_##N(FIELD) \
            } Waypoint_t; // Waypoint data structure

    DEFINE_WAYPOINT(6)

} // namespace Robotics
