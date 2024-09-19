#pragma once
#include <cstdint>

namespace StateMachine {
namespace RobotArm {

    enum class Event : uint8_t
    {
        EmergencyStop,
        Cancel,
        Done,
        Load,
        Teach,
        ProgramLoaded,
        Start,
        Pause,
        Resume,
        None
    };

    enum class StateId : uint8_t
    {
        Init,
        Ready,
        LoadProgram,
        Teach,
        ReadyAndLoaded,
        StartProgram,
        Execute,
        Paused,
        Manual,
        Calibrate,
        GoalBased,
        EmergencyStop
    };

    StateId stateTransMatrix(StateId currentState, Event event);

} // namespace RobotArm
} // namespace StateMachine
