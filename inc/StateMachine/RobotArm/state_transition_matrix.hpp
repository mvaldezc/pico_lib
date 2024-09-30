/***********************************************************************
 * @file	:	state_transition_matrix.hpp
 * @brief 	:	Robotic arm state transition matrix
 *              Defines the state transitions for a robotic arm.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <cstdint>

namespace StateMachine {
namespace RobotArm {

    /**
     * @brief Enum class representing the events that trigger state transitions of the robotic arm.
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
     * @brief Enum class representing the possible states of the robotic arm.
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
     * State diagram
     * @dot
     * digraph state_machine {
     *     node [shape=ellipse, style=filled, color=lightblue];
     *     edge [color=gray, fontcolor=black, fontsize=10];
     *     
     *     Init -> Ready [label="Done"];
     *     Ready -> LoadProgram [label="Load"];
     *     Ready -> Teach [label="Teach"];
     *     LoadProgram -> Ready [label="Cancel"];
     *     LoadProgram -> ReadyAndLoaded [label="ProgramLoaded"];
     *     Teach -> Ready [label="Cancel"];
     *     Teach -> ReadyAndLoaded [label="ProgramLoaded"];
     *     ReadyAndLoaded -> StartProgram [label="Start"];
     *     ReadyAndLoaded -> LoadProgram [label="Load"];
     *     ReadyAndLoaded -> Teach [label="Teach"];
     *     StartProgram -> ReadyAndLoaded [label="Cancel"];
     *     StartProgram -> Execute [label="Done"];
     *     Execute -> StartProgram [label="Start"];
     *     Execute -> Paused [label="Pause"];
     *     Execute -> ReadyAndLoaded [label="Done"];
     *     Execute -> ReadyAndLoaded [label="Cancel"];
     *     Paused -> StartProgram [label="Start"];
     *     Paused -> Execute [label="Resume"];
     *     Paused -> ReadyAndLoaded [label="Cancel"];
     * }
     * @enddot
     * @dot
     * digraph emergency_stop {
     *     node [shape=ellipse, style=filled, color=lightcoral];
     *     edge [color=red, fontcolor=black, fontsize=10];
     *     
     *     Init -> EmergencyStop [label="EmergencyStop"];
     *     Ready -> EmergencyStop [label="EmergencyStop"];
     *     LoadProgram -> EmergencyStop [label="EmergencyStop"];
     *     Teach -> EmergencyStop [label="EmergencyStop"];
     *     ReadyAndLoaded -> EmergencyStop [label="EmergencyStop"];
     *     StartProgram -> EmergencyStop [label="EmergencyStop"];
     *     Execute -> EmergencyStop [label="EmergencyStop"];
     *     Paused -> EmergencyStop [label="EmergencyStop"];
     * }
     * @enddot
     */

    /**
     * @brief State transition matrix of the robotic arm finite state machine.
     * @param[in] currentState Current state.
     * @param[in] event Event.
     * @return Next state.
     */
    StateId stateTransMatrix(StateId currentState, Event event);

} // namespace RobotArm
} // namespace StateMachine
