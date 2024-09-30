/***********************************************************************
 * @file	:	test_fsm_state_manager.cpp
 * @brief 	:	Test cases for FSMStateManager.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#include <gtest/gtest.h>
#include <thread>
#include "fsm_state_manager.hpp"
#include "mock_states.hpp"
#include "mock_factory.hpp"
#include <iostream>

namespace Tests {

    /**
     * @test Verifies the creation of a singleton instance of FSMStateManager.
     */
    TEST(FSMStateManagerTest, getInstance)
    {
        // create instance of FSMStateManager
        auto instance = StateMachine::RobotArm::FSMStateManager::getInstance();
        auto instance2 = StateMachine::RobotArm::FSMStateManager::getInstance();
        // check if instance is not null
        ASSERT_TRUE(instance != nullptr);
        // check if instance is of type FSMStateManager
        ASSERT_TRUE(dynamic_cast<StateMachine::RobotArm::FSMStateManager *>(instance) != nullptr);
        // check if instance is equal to instance2
        ASSERT_EQ(instance, instance2);
    }

    /**
     * @test Verifies the initial state of FSMStateManager.
     */
    TEST(FSMStateManagerTest, initialState)
    {
        // create instance of FSMStateManager
        auto instance = StateMachine::RobotArm::FSMStateManager::getInstance();
        // check if initial state is Init
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Init);
    }

    /**
     * @test Verifies a valid state transition occurs.
     */
    TEST(FSMStateManagerTest, validStateTransition)
    {
        // create instance of FSMStateManager
        auto instance = StateMachine::RobotArm::FSMStateManager::getInstance();
        // transition to state Ready
        instance->handleEvent(StateMachine::RobotArm::Event::Done);
        // check if current state is Ready
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Ready);
    }

    /**
     * @test Verifies an invalid state transition does not occur.
     */
    TEST(FSMStateManagerTest, invalidStateTransition)
    {
        // create instance of FSMStateManager
        auto instance = StateMachine::RobotArm::FSMStateManager::getInstance();
        // transition remains Init
        instance->handleEvent(StateMachine::RobotArm::Event::Cancel);
        // check if current state is Init
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Init);
        // trasition remains Init
        instance->handleEvent(StateMachine::RobotArm::Event::Load);
        // check if current state is Init
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Init);
    }

    /**
     * @test Verifies the state matrix transitions.
     */
    TEST(FSMStateManagerTest, stateMatrix)
    {
        // create instance of FSMStateManager
        auto instance = StateMachine::RobotArm::FSMStateManager::getInstance();
        // transition to state Ready
        instance->handleEvent(StateMachine::RobotArm::Event::Done);
        // check if current state is Ready
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Ready);
        // transition to state Teach
        instance->handleEvent(StateMachine::RobotArm::Event::Teach);
        // check if current state is Moving
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Teach);
        // transition to state Ready
        instance->handleEvent(StateMachine::RobotArm::Event::Cancel);
        // check if current state is Ready
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Ready);
        // transition to state LoadProgram
        instance->handleEvent(StateMachine::RobotArm::Event::Load);
        // check if current state is Moving
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::LoadProgram);
        // transition to state ReadyAndLoaded
        instance->handleEvent(StateMachine::RobotArm::Event::ProgramLoaded);
        // check if current state is Idle
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::ReadyAndLoaded);
        // transition to state StartProgram
        instance->handleEvent(StateMachine::RobotArm::Event::Start);
        // check if current state is StartProgram
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::StartProgram);
        // transition to state Execute
        instance->handleEvent(StateMachine::RobotArm::Event::Done);
        // check if current state is Execute
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Execute);
        // transition to state Paused
        instance->handleEvent(StateMachine::RobotArm::Event::Pause);
        // check if current state is Paused
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Paused);
        // transition to state Execute
        instance->handleEvent(StateMachine::RobotArm::Event::Resume);
        // check if current state is Execute
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::Execute);
        // transition to state ReadyAndLoaded
        instance->handleEvent(StateMachine::RobotArm::Event::Done);
        // check if current state is ReadyAndLoaded
        ASSERT_EQ(instance->getCurrentStateId(), StateMachine::RobotArm::StateId::ReadyAndLoaded);
    }

    /**
     * @test Verifies the concurrent creation of a singleton from multiple threads.
     */
    TEST(FSMStateManagerTest, concurrentSingletonCreation)
    {
        // create instance of FSMStateManager from multiple threads
        std::vector<std::thread> threads;
        // pointers to instance of FSMStateManager
        StateMachine::RobotArm::FSMStateManager * instances[3];
        for (int i = 0; i < 3; i++)
        {
            threads.push_back(std::thread([i, &instances]() {
                auto instance = dynamic_cast<StateMachine::RobotArm::FSMStateManager *>
                                (StateMachine::RobotArm::FSMStateManager::getInstance());
                instances[i] = instance;
                std::cout << "Thread " << i << " instance: " << instance << std::endl;
            }));
        }
        for (auto &thread : threads)
        {
            thread.join();
        }
        // check if instances are equal
        for (int i = 0; i < 3; i++)
        {
            ASSERT_EQ(instances[i], instances[0]);
        }
    }

    /**
     * @test Verifies the creation of states from the FSMStateFactory.
     */
    TEST(FSMStateFactoryTest, stateCreation)
    {
        using namespace StateMachine::RobotArm;
        // Create states
        auto init = FSMStateFactory::createState(StateId::Init, nullptr);
        auto ready = FSMStateFactory::createState(StateId::Ready, nullptr);

        // Check that states correspond to the correct state classes
        ASSERT_TRUE(dynamic_cast<States::Init *>(init.get()) != nullptr);
        ASSERT_TRUE(dynamic_cast<States::Ready *>(ready.get()) != nullptr);
    }

    /**
     * @test Verifies states are run and transitioned correctly.
     */
    TEST(StateManagerTest, stateRun)
    {
        // Create instance of StateManager with mock states
        auto instance = StateMachine::StateManager<MockStateId, MockEvent, mockStateTransMatrix>::getInstance(MockStateId::First);
        ASSERT_TRUE(instance != nullptr);
        ASSERT_TRUE(StateMachine::MockStateFactory::firstStateCreated);
        auto first_state = dynamic_cast<StateMachine::MockStates::First *>(StateMachine::MockStateFactory::pointers.first);
        ASSERT_TRUE(first_state != nullptr);

        ASSERT_EQ(first_state->enteredCnt, 0);
        ASSERT_EQ(first_state->runCnt, 0);
        ASSERT_EQ(first_state->exitedCnt, 0);

        // Run state
        instance->run();

        ASSERT_EQ(first_state->enteredCnt, 1);
        ASSERT_EQ(first_state->runCnt, 1);
        ASSERT_EQ(first_state->exitedCnt, 0);

        // Transition to state Second
        instance->handleEvent(MockEvent::Forward);
        ASSERT_EQ(instance->getCurrentStateId(), MockStateId::Second);

        // Run state
        instance->run();

        // Check if state is run
        ASSERT_TRUE(StateMachine::MockStateFactory::secondStateCreated);
        auto second_state = dynamic_cast<StateMachine::MockStates::Second *>(StateMachine::MockStateFactory::pointers.second);
        ASSERT_TRUE(second_state != nullptr);

        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.enteredCnt, 1);
        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.runCnt, 1);
        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.exitedCnt, 1);

        ASSERT_EQ(second_state->enteredCnt, 1);
        ASSERT_EQ(second_state->runCnt, 1);
        ASSERT_EQ(second_state->exitedCnt, 0);

        // Run state
        instance->run();

        ASSERT_EQ(second_state->enteredCnt, 1);
        ASSERT_EQ(second_state->runCnt, 2);
        ASSERT_EQ(second_state->exitedCnt, 0);

        // Transition to state Third
        instance->handleEvent(MockEvent::Forward);
        ASSERT_EQ(instance->getCurrentStateId(), MockStateId::Third);

        // Run state
        instance->run();

        auto third_state = dynamic_cast<StateMachine::MockStates::Third *>(StateMachine::MockStateFactory::pointers.third);
        ASSERT_TRUE(third_state != nullptr);

        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.enteredCnt, 1);
        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.runCnt, 2);
        ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.exitedCnt, 1);

        ASSERT_EQ(third_state->enteredCnt, 1);
        ASSERT_EQ(third_state->runCnt, 1);
        ASSERT_EQ(third_state->exitedCnt, 0);
    }

    /**
     * @test Verifies states are run and transitioned correctly with concurrent events.
     */
    TEST(StateManagerTest, stateRunWithEvent)
    {
        // Create instance of StateManager with mock states
        auto instance = StateMachine::StateManager<MockStateId, MockEvent, mockStateTransMatrix>::getInstance(MockStateId::First);

        auto first_state = dynamic_cast<StateMachine::MockStates::First *>(StateMachine::MockStateFactory::pointers.first);
        instance->run();

        auto eventThread = std::thread([&instance]() {
            // Transition to state Second
            instance->handleEvent(MockEvent::Forward);
        });

        auto runThread = std::thread([&instance]() {
            // Run state
            instance->run();
        });

        eventThread.join();
        runThread.join();

        // Check if state is run
        ASSERT_EQ(instance->getCurrentStateId(), MockStateId::Second);

        if (StateMachine::MockStateFactory::secondStateCreated)
        {
            auto second_state = dynamic_cast<StateMachine::MockStates::Second *>(StateMachine::MockStateFactory::pointers.second);
            ASSERT_TRUE(second_state != nullptr);

            ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.enteredCnt, 1);
            ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.runCnt, 1);
            ASSERT_EQ(StateMachine::MockStateFactory::lastStateData.exitedCnt, 1);

            ASSERT_EQ(second_state->enteredCnt, 1);
            ASSERT_EQ(second_state->runCnt, 1);
            ASSERT_EQ(second_state->exitedCnt, 0);
            std::cout << "First 1, Second 1" << std::endl;
        }
        else
        {
            ASSERT_TRUE(first_state != nullptr);
            ASSERT_EQ(first_state->enteredCnt, 1);
            ASSERT_EQ(first_state->runCnt, 2);
            ASSERT_EQ(first_state->exitedCnt, 0);
            std::cout << "First 2, Second 0" << std::endl;
        }
    }

} // namespace Tests
