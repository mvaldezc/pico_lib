/***********************************************************************
 * @file	:	state_factory.hpp
 * @brief 	:	StateFactory class
 *              Template for creating states of a state machine.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "state.hpp"
#include <memory>

namespace StateMachine {

    // State transition matrix data type
    template <typename StateId, typename Event>
    using StateTransMatrix = StateId (*)(StateId, Event);

    // State forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class State;

    // StateManager forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class StateManager;

    /**
     * @class StateFactory
     * @brief Creates state objects of a finite state machine following the factory design pattern.
     * 
     * @tparam StateId Enum class representing the possible states.
     * @tparam Event Enum class representing the events that trigger state transitions.
     * @tparam stateTransMatrix State transition matrix function pointer.
     * 
     */
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class StateFactory
    {
        public:
            /**
             * @brief Create a state object corresponding to the given state id.
             * @param[in] sId State id.
             * @param[in] sManager State manager.
             * @return Unique pointer to the state object.
             */
            static std::unique_ptr<State<StateId, Event, stateTransMatrix>> createState(StateId sId, StateManager<StateId, Event, stateTransMatrix> *sManager)
            {
                return nullptr;
            };
    };

} // namespace StateMachine
