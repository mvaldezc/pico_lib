#pragma once
#include "state_manager.hpp"

namespace StateMachine {

    // State transition matrix data type
    template <typename StateId, typename Event>
    using StateTransMatrix = StateId(*)(StateId, Event);

    // StateManager forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class StateManager;

    /**
     * @class State
     *
     * @brief Abstract class representing a state of a finite state machine.
     *
     * @tparam StateId Enum class representing the possible states.
     * @tparam Event Enum class representing the events that trigger state transitions.
     * @tparam FSM_STM State transition matrix function pointer.
     *
     */
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class State
    {
    protected:
        using StateManager_ = StateManager<StateId, Event, FSM_STM>;

    public:
        State(StateId sId, StateManager_ * sManager)
            : currentState(sId), manager(sManager) {}; // Enforce child to call this constructor
        virtual void run() = 0;
        virtual void onEnter() = 0;
        virtual void onExit() = 0;
        StateId getStateId() const noexcept
        {
            return currentState;
        }

        virtual ~State() {};

    protected:
        const StateId currentState;
        const StateManager_ * manager;
    };
}