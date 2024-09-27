/***********************************************************************
 * @file	:	states_behavior.hpp
 * @brief 	:	RobotArm machine states
 *              Defines the behavior of the states for a RobotArm.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "state.hpp"
#include "state_transition_matrix.hpp"

namespace StateMachine {
namespace RobotArm {
namespace States {

        class Init : public State<StateId, Event, stateTransMatrix>
        {
            public:
                Init(StateManager_ * instance) : State(StateId::Init, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class Ready : public State<StateId, Event, stateTransMatrix>
        {
            public:
                Ready(StateManager_ * instance) : State(StateId::Ready, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class LoadProgram : public State<StateId, Event, stateTransMatrix>
        {
            public:
                LoadProgram(StateManager_ * instance) : State(StateId::LoadProgram, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class Teach : public State<StateId, Event, stateTransMatrix>
        {
            public:
                Teach(StateManager_ * instance) : State(StateId::Teach, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class ReadyAndLoaded : public State<StateId, Event, stateTransMatrix>
        {
            public:
                ReadyAndLoaded(StateManager_ * instance) : State(StateId::ReadyAndLoaded, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class StartProgram : public State<StateId, Event, stateTransMatrix>
        {
            public:
                StartProgram(StateManager_ * instance) : State(StateId::StartProgram, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class Execute : public State<StateId, Event, stateTransMatrix>
        {
            public:
                Execute(StateManager_ * instance) : State(StateId::Execute, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class Paused : public State<StateId, Event, stateTransMatrix>
        {
            public:
                Paused(StateManager_ * instance) : State(StateId::Paused, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };

        class EmergencyStop : public State<StateId, Event, stateTransMatrix>
        {
            public:
                EmergencyStop(StateManager_ * instance) : State(StateId::EmergencyStop, instance) {};
                void run() override;
                void onEnter() override;
                void onExit() override;
        };
} // namespace States
} // namespace RobotArm
} // namespace StateMachine
