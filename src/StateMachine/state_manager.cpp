#include "StateMachine/state_manager.hpp"
using namespace StateMachine;

void StateManager::setAction(Action && instruction) noexcept
{
    critical_section_enter_blocking(&stateManagerLock);
    instance->instructionBuffer = instruction;
    instance->machineData.state.actionNotRequired = false;
    critical_section_exit(&stateManagerLock);
}

void StateManager::setEmergencyStop() noexcept
{
    critical_section_enter_blocking(&stateManagerLock);
    instance->machineData.state.emergencyStop = true;
    instance->machineData.state.actionNotRequired = false;
    critical_section_exit(&stateManagerLock);
}

MachineState StateManager::getMachineState() noexcept
{
    critical_section_enter_blocking(&stateManagerLock);
    MachineState returnVal = machineData.state.mode;
    critical_section_exit(&stateManagerLock);
    return returnVal;
}

void StateManager::stateTransition()
{
    critical_section_enter_blocking(&stateManagerLock);

    // Only if action needed, do something
    // Only if emergency is enabled, ignore everything else
    switch (machineData.key) // Current state
    {
        // Any state -> EmergencyStop
        case (EMERGENCY_ENABLED | static_cast<uint32_t>(MachineState::Off)):
        case (EMERGENCY_ENABLED | static_cast<uint32_t>(MachineState::LoadProgram)):
        case (EMERGENCY_ENABLED | static_cast<uint32_t>(MachineState::WaitStart)):
        case (EMERGENCY_ENABLED | static_cast<uint32_t>(MachineState::ExecuteProgram)):
        case (EMERGENCY_ENABLED | static_cast<uint32_t>(MachineState::ProgramStop)):
            machineData.state.mode = MachineState::EmergencyStop;
            break;
        
        case (static_cast<uint32_t>(MachineState::Off)):
            switch (instructionBuffer)
            {
                case Action::Load : // Off -> LoadProgram
                    programLoaded = false;
                    machineData.state.mode = MachineState::LoadProgram;
                    break;
                case Action::Program: // Off -> WaitStart
                    if(programLoaded)
                    {
                        machineData.state.mode = MachineState::WaitStart;
                    }
                    break;
                default:
                    break;
            }
            break;
            
        case (static_cast<uint32_t>(MachineState::LoadProgram)):
            switch (instructionBuffer)
            {
                case Action::Done: // LoadProgram -> WaitStart
                    programLoaded = true;
                    machineData.state.mode = MachineState::WaitStart;
                    break;
                case Action::Cancel: // LoadProgram -> Off
                    machineData.state.mode = MachineState::Off;
                    break;
                default:
                    break;
            }
            break;

        case (static_cast<uint32_t>(MachineState::WaitStart)):
            switch (instructionBuffer)
            {
                case Action::Cancel: // Waitstart -> Off
                    machineData.state.mode = MachineState::Off;
                    break;
                case Action::Start: // Waitstart -> ExecuteProgram
                    machineData.state.mode = MachineState::ExecuteProgram;
                    break;
                default:
                    break;
            }
            break;

        case (static_cast<uint32_t>(MachineState::ExecuteProgram)):
            switch (instructionBuffer)
            {
                case Action::Stop:  // ExecuteProgram -> ProgramStop
                    machineData.state.mode = MachineState::ProgramStop;
                    break;
                case Action::Done:  // ExecuteProgram -> Off
                    machineData.state.mode = MachineState::Off;
                    break;
                default:
                    break;
            }
            break;

        case (static_cast<uint32_t>(MachineState::ProgramStop)):
            switch (instructionBuffer)
            {
                case Action::Start:     // ProgramStop -> ExecuteProgram
                    machineData.state.mode = MachineState::ExecuteProgram;
                    break;
                case Action::Cancel:    // ProgramStop -> Off
                    machineData.state.mode = MachineState::Off;
                    break;
                default:
                    break;
            }
            break;

        case (static_cast<uint32_t>(MachineState::EmergencyStop)):
            // Needs to be restarted
            break;

        default:
            break;
    }
    machineData.state.actionNotRequired = true; // Action has been processed
    instance->instructionBuffer = Action::None; // Clear the buffer

    critical_section_exit(&stateManagerLock);
}

// Initialize static member
StateManager * StateManager::instance{nullptr};
