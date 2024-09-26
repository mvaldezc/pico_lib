/***********************************************************************
 * @file	:	fsm_state_factory.hpp
 * @brief 	:	RobotArm StateFactory specialization
 *              Library to create states of a RobotArm state machine.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "state_factory.hpp"
#include "states_behavior.hpp"

namespace StateMachine{

    /**
     * @class StateFactory
     * @brief Template specialization of the StateFactory class for the RobotArm finite state machine.
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
