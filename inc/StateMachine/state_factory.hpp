#pragma once
#include "state.hpp"
#include <memory>

namespace StateMachine {

    // State transition matrix data type
    template <typename StateId, typename Event>
    using StateTransMatrix = StateId (*)(StateId, Event);

    // State forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class State;

    // StateManager forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class StateManager;

    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class StateFactory
    {
        public:
            static std::unique_ptr<State<StateId, Event, FSM_STM>> createState(StateId sId, StateManager<StateId, Event, FSM_STM> *sManager)
            {
                return nullptr;
            };
    };

} // namespace StateMachine
