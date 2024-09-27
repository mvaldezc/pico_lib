/***********************************************************************
 * @file	:	state_transition_matrix.hpp
 * @brief 	:	RobotArm state transition matrix
 *              Defines the state transitions for a RobotArm.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <cstdint>

namespace StateMachine {
namespace RobotArm {

    /**
     * @brief Enum class representing the events that trigger state transitions of the RobotArm.
     */
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

    /**
     * @brief Enum class representing the possible states of the RobotArm.
     */
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

    /**
     * @brief State transition matrix of the RobotArm finite state machine.
     * @param[in] currentState Current state.
     * @param[in] event Event.
     * @return StateId Next state.
     */
    StateId stateTransMatrix(StateId currentState, Event event);

} // namespace RobotArm
} // namespace StateMachine
