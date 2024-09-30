/***********************************************************************
 * @file	:	fsm_state_factory.hpp
 * @brief 	:	Robotic arm StateFactory specialization
 *              Library to create states of a robotic arm state machine.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "state_factory.hpp"
#include "states_behavior.hpp"

namespace StateMachine{

    /**
     * @brief Template specialization of the StateFactory class for the robotic arm finite state machine.
     */
    template <>
    class StateFactory<RobotArm::StateId, RobotArm::Event, RobotArm::stateTransMatrix>
    {
        private:
            using StateManager_ = StateManager<RobotArm::StateId, RobotArm::Event, RobotArm::stateTransMatrix>;
        public:
            static std::unique_ptr<State<RobotArm::StateId, RobotArm::Event, RobotArm::stateTransMatrix>>
                createState(RobotArm::StateId sId, StateManager_ * sManager);
    };

    namespace RobotArm
    {
        using FSMStateFactory = StateMachine::StateFactory<RobotArm::StateId, RobotArm::Event, RobotArm::stateTransMatrix>;
    } // namespace RobotArm

} // namespace StateMachine
