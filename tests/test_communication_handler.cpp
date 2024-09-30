/***********************************************************************
 * @file	:	test_communication_handler.cpp
 * @brief 	:	Test cases for communication handler.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

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

namespace Communication{
    namespace RobotArm{
        extern StateMachine::StateManager<StateMachine::RobotArm::StateId,StateMachine::RobotArm::Event, StateMachine::RobotArm::stateTransMatrix> * stateManager;
    }
}

namespace Utilities {
    extern bool resetCalled;
}

namespace Tests {

    /**
     * @brief Mock class for the FSMStateManager.
     */
    class MockStateManager : public StateMachine::RobotArm::FSMStateManager 
    {
        public:
            MOCK_METHOD(void, handleEvent, (StateMachine::RobotArm::Event && event), (override));
            MOCK_METHOD(StateMachine::RobotArm::StateId, getPerformingStateId, (), (const, override, noexcept));
    };

    /**
     * @brief Mock class for the Trajectory.
     */
    class MockTrajectory : public Robotics::Trajectory 
    {
        public:
            MOCK_METHOD(void, saveWaypoints, (const uint8_t* rawWaypoints, size_t size), (override));
    };

    /**
     * @brief Test fixture for the CommunicationHandler.
     */
    class CommunicationHandlerTest : public ::testing::Test 
    {
        protected:
            /**
             * @brief Build required test objects
             */
            void SetUp() override {
                Communication::RobotArm::stateManager = &mockStateManager;
                programData = std::make_shared<MockTrajectory>();
                installDataContainer(programData);
            }

            /**
             * @brief Delete test objects
             */
            void TearDown() override {
                uninstallDataContainer();
            }

            MockStateManager mockStateManager;
            std::shared_ptr<MockTrajectory> programData;
    };

    /**
     * @test Verifies that the data container is correctly installed.
     */
    TEST(DataContainer, install)
    {
        std::shared_ptr<MockTrajectory> cont = std::make_shared<MockTrajectory>();
        installDataContainer(cont);
        ASSERT_EQ(getContainer(), cont);
    }

    /**
     * @test Verifies that the data container is correctly uninstalled.
     */
    TEST(DataContainer, uninstall)
    {
        std::shared_ptr<MockTrajectory> cont = std::make_shared<MockTrajectory>();
        installDataContainer(cont);
        uninstallDataContainer();
        ASSERT_EQ(getContainer(), nullptr);
    }

    /**
     * @test Verifies that the emergency stop callback triggers the correct event.
     */
    TEST_F(CommunicationHandlerTest, EmergencyStopCallback) 
    {
        EXPECT_CALL(mockStateManager, handleEvent(StateMachine::RobotArm::Event::EmergencyStop));
        emergencyStopCallback(nullptr, 0);
    }

    /**
     * @test Verifies that the program data callback saves waypoints when the state is LoadProgram.
     */
    TEST_F(CommunicationHandlerTest, ProgramDataCallback) 
    {
        uint8_t data[] = {1, 2, 3, 4};
        size_t dataLength = sizeof(data);

        EXPECT_CALL(mockStateManager, getPerformingStateId())
            .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::LoadProgram; }));
        EXPECT_CALL(*programData, saveWaypoints(data, dataLength));
        programDataCallback(data, dataLength);
    }

    /**
     * @test Verifies that the program data callback does not save waypoints when there is no data container.
     */
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

    /**
     * @test Verifies that the program data callback does not save waypoints when the state is not LoadProgram.
     */
    TEST_F(CommunicationHandlerTest, ProgramDataCallbackWrongState) 
    {
        uint8_t data[] = {1, 2, 3, 4};
        size_t dataLength = sizeof(data);

        EXPECT_CALL(mockStateManager, getPerformingStateId())
            .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::Init; }));
        EXPECT_CALL(*programData, saveWaypoints(data, dataLength)).Times(0);
        programDataCallback(data, dataLength);
    }

    /**
     * @test Verifies that the program data callback does not save waypoints when there is no data.
     */
    TEST_F(CommunicationHandlerTest, ProgramDataCallbackNoData) 
    {
        size_t dataLength = 0;

        EXPECT_CALL(mockStateManager, getPerformingStateId())
            .WillOnce(Invoke([]() { return StateMachine::RobotArm::StateId::LoadProgram; }));
        EXPECT_CALL(*programData, saveWaypoints(nullptr, dataLength)).Times(0);
        programDataCallback(nullptr, dataLength);
    }

    /**
     * @test Verifies that the reset callback triggers the reset function.
     */
    TEST_F(CommunicationHandlerTest, ResetCallback) 
    {
        // expect call of reset function from reset.hpp not belonging to any class
        ASSERT_EQ(Utilities::resetCalled, false);
        resetCallback(nullptr, 0);
        ASSERT_EQ(Utilities::resetCalled, true);
    }

    /**
     * @test Verifies that the RX callback triggers the correct event.
     */
    TEST_F(CommunicationHandlerTest, RxCallback) 
    {
        uint8_t data[] = {1, 2, 3, 4};
        size_t dataLength = sizeof(data);

        EXPECT_CALL(mockStateManager, handleEvent(StateMachine::RobotArm::Event::EmergencyStop));
        rxCallback(RAW(RxIds::EMERGENCY_STOP), dataLength, data);
    }

    /**
     * @test Verifies that the RX callback does not trigger any event for an undefined message ID.
     */
    TEST_F(CommunicationHandlerTest, RxCallbackUndefinedMessageId) 
    {
        uint8_t data[] = {1, 2, 3, 4};
        size_t dataLength = sizeof(data);

        EXPECT_CALL(mockStateManager, handleEvent(_)).Times(0);
        rxCallback(0xFF, dataLength, data);
    }

} // namespace Tests
