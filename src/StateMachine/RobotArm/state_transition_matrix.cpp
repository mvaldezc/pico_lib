#include "state_transition_matrix.hpp"
using namespace StateMachine::RobotArm;

StateId StateMachine::RobotArm::stateTransMatrix(StateId currentState, Event event)
{
    switch (currentState)
    {
        case StateId::Init:
            switch (event) 
            {
                case Event::Done:           return StateId::Ready;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::Ready:
            switch (event) 
            {
                case Event::Load:           return StateId::LoadProgram;
                case Event::Teach:          return StateId::Teach;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::LoadProgram:
            switch (event) 
            {
                case Event::Cancel:         return StateId::Ready;
                case Event::ProgramLoaded:  return StateId::ReadyAndLoaded;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::Teach:
            switch (event) 
            {
                case Event::Cancel:         return StateId::Ready;
                case Event::ProgramLoaded:  return StateId::ReadyAndLoaded;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::ReadyAndLoaded:
            switch (event) 
            {
                case Event::Start:          return StateId::StartProgram;
                case Event::Load:           return StateId::LoadProgram;
                case Event::Teach:          return StateId::Teach;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::StartProgram:
            switch (event) 
            {
                case Event::Cancel:         return StateId::ReadyAndLoaded;
                case Event::Done:           return StateId::Execute;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::Execute:
            switch (event) 
            {
                case Event::Start:          return StateId::StartProgram;
                case Event::Pause:          return StateId::Paused;
                case Event::Done:           return StateId::ReadyAndLoaded;
                case Event::Cancel:         return StateId::ReadyAndLoaded;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::Paused:
            switch (event) 
            {
                case Event::Start:          return StateId::StartProgram;
                case Event::Resume:         return StateId::Execute;
                case Event::Cancel:         return StateId::ReadyAndLoaded;
                case Event::EmergencyStop:  return StateId::EmergencyStop;
                default:                    return currentState;
            }
        case StateId::EmergencyStop:
            switch (event) 
            {
                default:                    return currentState;
            }
        default:
            return currentState;
    }   
};
