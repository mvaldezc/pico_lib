#include "communication_handler.hpp"
#include "fsm_state_manager.hpp"
#include "reset.hpp"

namespace Communication{
namespace RobotArm{

    using StateMachine::StateManager;
    using StateMachine::RobotArm::FSMStateManager;
    using StateMachine::RobotArm::Event;
    using StateMachine::RobotArm::StateId;

    static std::shared_ptr<Robotics::Trajectory> programData{nullptr};
    auto stateManager = FSMStateManager::getInstance();

    void emergencyStopCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::EmergencyStop);
    }

    void cancelOperationCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Cancel);
    }

    void loadProgramCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Load);
    }

    void programDataCallback(const uint8_t * msgData, const size_t dataLength)
    {
        if (stateManager->getPerformingStateId() == StateId::LoadProgram)
        {
            if (programData != nullptr && msgData != nullptr && dataLength > 0)
            {
                    programData->saveWaypoints(msgData, dataLength);
            }
        }
    }

    void teachProgramCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Teach);
    }

    void startProgramCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Start);
    }

    void pauseProgramCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Pause);
    }

    void resumeProgramCallback(const uint8_t * msgData, const size_t dataLength)
    {
        stateManager->handleEvent(Event::Resume);
    }

    void resetCallback(const uint8_t * msgData, const size_t dataLength)
    {
        Utilities::reset();
    }

    void rxCallback(RxMessageId msgId, size_t dataLength, uint8_t * msgData)
    {
        // If message ID exist in messageDictionary
        if (messageDictionary.count(msgId) == 1)
        {
            // If message handler exists
            if (messageDictionary.at(msgId) != nullptr)
            {
                // Call message handler
                messageDictionary.at(msgId)(msgData, dataLength);
            }
        }
    }

    void txCallback(uint8_t *msgData)
    {
        *msgData = 0x07;
    }

    void installDataContainer(std::shared_ptr<Robotics::Trajectory> via_points)
    {
        programData = via_points;
    }

    void uninstallDataContainer()
    {
        programData.reset();
    }

} // namespace RobotArm
} // namespace Communication
