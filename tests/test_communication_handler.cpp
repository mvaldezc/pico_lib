#include "communication_handler.hpp"
#include "reset.hpp"
#include "fsm_state_manager.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

using namespace Communication::RobotArm;
using namespace Communication;
using namespace Utilities;
using ::testing::_;
using ::testing::Invoke;

// Mock classes for dependencies
class MockStateManager : public StateMachine::RobotArm::FSMStateManager {
public:
    MOCK_METHOD(void, handleEvent, (StateMachine::RobotArm::Event && event), (override));
    MOCK_METHOD(StateMachine::RobotArm::StateId, getPerformingStateId, (), (const, override, noexcept));
};

class MockTrajectory : public Robotics::Trajectory {
public:
    MOCK_METHOD(void, saveWaypoints, (const uint8_t* rawWaypoints, size_t size), (override));
};

namespace Communication{
    namespace RobotArm{
        extern StateMachine::StateManager<StateMachine::RobotArm::StateId,StateMachine::RobotArm::Event, StateMachine::RobotArm::stateTransMatrix> * stateManager;
    }
}

namespace Utilities {
    extern bool resetCalled;
}

// Test fixture
class CommunicationHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        stateManager = &mockStateManager;
        programData = std::make_shared<MockTrajectory>();
        installDataContainer(programData);
    }

    void TearDown() override {
        uninstallDataContainer();
    }

    MockStateManager mockStateManager;
    std::shared_ptr<MockTrajectory> programData;
};

TEST(DataContainer, install)
{
    std::shared_ptr<MockTrajectory> cont = std::make_shared<MockTrajectory>();
    installDataContainer(cont);
    ASSERT_EQ(getContainer(), cont);
}

TEST(DataContainer, uninstall)
{
    std::shared_ptr<MockTrajectory> cont = std::make_shared<MockTrajectory>();
    installDataContainer(cont);
    uninstallDataContainer();
    ASSERT_EQ(getContainer(), nullptr);
}

TEST_F(CommunicationHandlerTest, EmergencyStopCallback) 
{
    EXPECT_CALL(mockStateManager, handleEvent(StateMachine::RobotArm::Event::EmergencyStop));
    emergencyStopCallback(nullptr, 0);
}

TEST_F(CommunicationHandlerTest, ProgramDataCallback) 
{
    uint8_t data[] = {1, 2, 3, 4};
    size_t dataLength = sizeof(data);

    EXPECT_CALL(mockStateManager, getPerformingStateId())
        .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::LoadProgram; }));
    EXPECT_CALL(*programData, saveWaypoints(data, dataLength));
    programDataCallback(data, dataLength);
}

TEST_F(CommunicationHandlerTest, ProgramDataCallbackNoDataContainer) 
{
    uninstallDataContainer();
    uint8_t data[] = {1, 2, 3, 4};
    size_t dataLength = sizeof(data);

    EXPECT_CALL(mockStateManager, getPerformingStateId())
        .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::LoadProgram; }));
    EXPECT_CALL(*programData, saveWaypoints(data, dataLength)).Times(0);
    programDataCallback(data, dataLength);
}

TEST_F(CommunicationHandlerTest, ProgramDataCallbackWrongState) 
{
    uint8_t data[] = {1, 2, 3, 4};
    size_t dataLength = sizeof(data);

    EXPECT_CALL(mockStateManager, getPerformingStateId())
        .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::Init; }));
    EXPECT_CALL(*programData, saveWaypoints(data, dataLength)).Times(0);
    programDataCallback(data, dataLength);
}

TEST_F(CommunicationHandlerTest, ProgramDataCallbackNoData) 
{
    size_t dataLength = 0;

    EXPECT_CALL(mockStateManager, getPerformingStateId())
        .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::LoadProgram; }));
    EXPECT_CALL(*programData, saveWaypoints(nullptr, dataLength)).Times(0);
    programDataCallback(nullptr, dataLength);
}

TEST_F(CommunicationHandlerTest, ResetCallback) 
{
    // expect call of reset function from reset.hpp not belonging to any class
    ASSERT_EQ(resetCalled, false);
    resetCallback(nullptr, 0);
    ASSERT_EQ(resetCalled, true);
}

TEST_F(CommunicationHandlerTest, RxCallback) 
{
    uint8_t data[] = {1, 2, 3, 4};
    size_t dataLength = sizeof(data);

    EXPECT_CALL(mockStateManager, handleEvent(StateMachine::RobotArm::Event::EmergencyStop));
    rxCallback(RAW(RxIds::EMERGENCY_STOP), dataLength, data);
}

TEST_F(CommunicationHandlerTest, RxCallbackUndefinedMessageId) 
{
    uint8_t data[] = {1, 2, 3, 4};
    size_t dataLength = sizeof(data);

    EXPECT_CALL(mockStateManager, handleEvent(_)).Times(0);
    rxCallback(0xFF, dataLength, data);
}
