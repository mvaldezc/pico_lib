#pragma once
#include "state_factory.hpp"
#include "mock_states.hpp"

namespace StateMachine{
    namespace MockStates
    {
        class First: public State<MockStateId, MockEvent, mockStateTransMatrix>
        {
            public:
                First(MockStateId sId, StateManager<MockStateId, MockEvent, mockStateTransMatrix> *sManager) : State(sId, sManager) {};
                void run() override
                {
                    runCnt++;
                };
                void onEnter() override
                {
                    enteredCnt++;
                };
                void onExit() override
                {
                    exitedCnt++;
                };
                int enteredCnt = 0;
                int exitedCnt = 0;
                int runCnt = 0;
        };

        class Second: public State<MockStateId, MockEvent, mockStateTransMatrix>
        {
            public:
                Second(MockStateId sId, StateManager<MockStateId, MockEvent, mockStateTransMatrix> *sManager) : State(sId, sManager) {};
                void run() override
                {
                    runCnt++;
                };
                void onEnter() override
                {
                    enteredCnt++;
                };
                void onExit() override
                {
                    exitedCnt++;
                };
                int enteredCnt = 0;
                int exitedCnt = 0;
                int runCnt = 0;
        };

        class Third: public State<MockStateId, MockEvent, mockStateTransMatrix>
        {
            public:
                Third(MockStateId sId, StateManager<MockStateId, MockEvent, mockStateTransMatrix> *sManager) : State(sId, sManager) {};
                void run() override
                {
                    runCnt++;
                };
                void onEnter() override
                {
                    enteredCnt++;
                };
                void onExit() override
                {
                    exitedCnt++;
                };
                int enteredCnt = 0;
                int exitedCnt = 0;
                int runCnt = 0;
        };

    } // namespace MockStates

    template <>
    class StateFactory<MockStateId, MockEvent, mockStateTransMatrix>
    {
        private:
            using StateManager_ = StateManager<MockStateId, MockEvent, mockStateTransMatrix>;

        public:
            static std::unique_ptr<State<MockStateId, MockEvent, mockStateTransMatrix>>
            createState(MockStateId sId, StateManager_ *sManager)
            {
                if (pointers.first != nullptr)
                {
                    lastStateData.enteredCnt = pointers.first->enteredCnt;
                    lastStateData.runCnt = pointers.first->runCnt;
                    lastStateData.exitedCnt = pointers.first->exitedCnt;
                }
                else if (pointers.second != nullptr)
                {   
                    lastStateData.enteredCnt = pointers.second->enteredCnt;
                    lastStateData.runCnt = pointers.second->runCnt;
                    lastStateData.exitedCnt = pointers.second->exitedCnt;
                }
                else if (pointers.third != nullptr)
                {
                    lastStateData.enteredCnt = pointers.third->enteredCnt;
                    lastStateData.runCnt = pointers.third->runCnt;
                    lastStateData.exitedCnt = pointers.third->exitedCnt;
                }

                switch (sId)
                {
                    case MockStateId::First:
                        firstStateCreated = true;
                        pointers.first = new MockStates::First(sId, sManager);
                        pointers.second = nullptr;
                        pointers.third = nullptr;
                        return std::unique_ptr<MockStates::First>(pointers.first);
                    case MockStateId::Second:
                        secondStateCreated = true;
                        pointers.second = new MockStates::Second(sId, sManager);
                        pointers.third = nullptr;
                        pointers.first = nullptr;
                        return std::unique_ptr<MockStates::Second>(pointers.second);
                    case MockStateId::Third:
                        thirdStateCreated = true;
                        pointers.third = new MockStates::Third(sId, sManager);
                        pointers.first = nullptr;
                        pointers.second = nullptr;
                        return std::unique_ptr<MockStates::Third>(pointers.third);
                    default:
                        return nullptr;
                }
            };

            typedef struct
            {
                MockStates::First * first;
                MockStates::Second * second;
                MockStates::Third * third;
            } statePointers;

            static statePointers pointers;
            static bool firstStateCreated;
            static bool secondStateCreated;
            static bool thirdStateCreated;

            typedef struct
            {
                int enteredCnt;
                int exitedCnt;
                int runCnt;
            } LastStateData;

            static LastStateData lastStateData;
    };

    using MockStateFactory = StateFactory<MockStateId, MockEvent, mockStateTransMatrix>;

    MockStateFactory::statePointers MockStateFactory::pointers{nullptr, nullptr, nullptr};
    bool MockStateFactory::firstStateCreated = false;
    bool MockStateFactory::secondStateCreated = false;
    bool MockStateFactory::thirdStateCreated = false;

    MockStateFactory::LastStateData MockStateFactory::lastStateData{0, 0, 0};

} // namespace StateMachine