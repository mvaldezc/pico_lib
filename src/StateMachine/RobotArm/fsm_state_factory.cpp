#include "fsm_state_factory.hpp"
using namespace StateMachine::RobotArm::States;
using namespace StateMachine::RobotArm;
using namespace StateMachine;

std::unique_ptr<State<StateId, Event, stateTransMatrix>> FSMStateFactory::createState(StateId sId, StateManager_ * sManager)
{
    switch (sId)
    {
        case StateId::Init:
            return std::make_unique<Init>(sManager);
        case StateId::Ready:
            return std::make_unique<Ready>(sManager);
        case StateId::LoadProgram:
            return std::make_unique<LoadProgram>(sManager);
        case StateId::Teach:
            return std::make_unique<Teach>(sManager);
        case StateId::ReadyAndLoaded:
            return std::make_unique<ReadyAndLoaded>(sManager);
        case StateId::StartProgram:
            return std::make_unique<StartProgram>(sManager);
        case StateId::Execute:
            return std::make_unique<Execute>(sManager);
        case StateId::Paused:
            return std::make_unique<Paused>(sManager);
        case StateId::EmergencyStop:
            return std::make_unique<EmergencyStop>(sManager);
        default:
            return nullptr;
    }
}
