#pragma once
#include "state_factory.hpp"
#include "states.hpp"

namespace StateMachine{

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
