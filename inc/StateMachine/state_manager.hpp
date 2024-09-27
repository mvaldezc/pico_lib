/***********************************************************************
 * @file	:	state_manager.hpp
 * @brief 	:	StateManager class
 *              Template library for finite state machine functionality.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <memory>
#include <atomic>
#include "state.hpp"
#include "state_factory.hpp"
#include "pico/critical_section.h"
#include "pico/mutex.h"
#include "lock_guard.hpp"

namespace StateMachine {

    // Type alias for the state transition matrix function
    template <typename StateId, typename Event>
    using StateTransMatrix = StateId (*)(StateId, Event);

    // Forward declaration of State
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class State;

    // Forward declaration of StateFactory
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class StateFactory;

    // equivalent to static mutex declaration + mutex_init()
    auto_init_mutex(createStateManagerMutex);

    /**
     * @class StateManager
     * @brief Manages a finite state machine as a thread/ISR-safe leaky singleton.
     * 
     * The StateManager handles state transitions based on events, manages the current state, 
     * and executes the state's behavior. The class is thread/ISR-safe and uses a critical 
     * section for concurrent access to non-reentrant methods.
     * 
     * Template parameters allow defining the sets of states, events, and state transitions.
     * It runs state-behavior folowing the state-machine design pattern.
     * NOTE: For template classes, all member function definitions must be in the header file. 
     * 
     * @tparam StateId Enum class representing the possible states.
     * @tparam Event Enum class representing the events that trigger state transitions.
     * @tparam stateTransMatrix State transition matrix function pointer.
     * 
     */
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    class StateManager
    {   
        protected:
            using State_ = State<StateId, Event, stateTransMatrix>;

            /**
             * @brief Private constructor to prevent multiple instances.
             * @param[in] sId Initial state id.
             */
            StateManager(StateId sId) : currentStateId(sId)
            {
                critical_section_init(&stateManagerLock);
                // Create initial state object
                state = StateFactory<StateId, Event, stateTransMatrix>::createState(sId, this);
            }

            /**
             * @brief Transitions to a new state.
             * @param[in] newState Unique pointer to the new state.
             */
            void stateTransition(std::unique_ptr<State_> && newState)
            {
                state = std::move(newState);
            }

            static StateManager * instance;
            critical_section_t stateManagerLock;
            StateId currentStateId;
            std::unique_ptr<State_> state;
            std::atomic_bool stateChanged = false;
            std::atomic_bool initialized = false;

        public:
            
            /**
             * @brief Retrieves the singleton instance.
             * @param[in] sId Initial state ID.
             * @return Pointer to the StateManager.
             */
            static StateManager * getInstance(StateId sId)
            {
                // Acquire mutex to avoid creation of multiple instances of class
                // by concurrent calls of this method.
                lock_guard<mutex_t> mutexWrapper(createStateManagerMutex);
                // If unique StateManager instance doesn't exist, create it.
                if (instance == nullptr)
                {
                    instance = new StateManager(sId);
                }
                // Return singleton.
                return instance;
            }

            /**
             * @brief Handle an event and triggers a state transition if necessary.
             * @param[in] event Rvalue of event to handle.
             */
            virtual void handleEvent(Event && event)
            {
                auto lastStateId = currentStateId;
                critical_section_enter_blocking(&stateManagerLock);
                // Update the current state based on the event using the state transition matrix
                currentStateId = stateTransMatrix(currentStateId, event);
                critical_section_exit(&stateManagerLock);
                // If state has changed
                stateChanged = (lastStateId != currentStateId);
            }

            /**
             * @brief Monitor state changes, enter, run and exit states. 
             *        To be called in a loop, from a single thread.
             */
            void run()
            {
                // If state has changed
                if (stateChanged)
                {
                    // Exit current state
                    state->onExit();
                    // Create new state
                    auto newState = StateFactory<StateId, Event, stateTransMatrix>::createState(currentStateId, this);
                    // Transition to new state
                    stateTransition(std::move(newState));
                    // Enter new state
                    state->onEnter();
                    stateChanged = false;
                }

                // Enter initial state if not done yet
                if(initialized == false)
                {   
                    state->onEnter();
                    initialized = true;
                }

                // Run current state
                state->run();
            }

            /**
             * @brief Get the current state id.
             * @return Current state id.
             */
            StateId getCurrentStateId() const noexcept
            {
                return currentStateId;
            }

            /**
             * @brief Get the performing state id.
             * @return Performing state id.
             */
            virtual StateId getPerformingStateId() const noexcept
            {
                return state->getStateId();
            }

            // Singleton shouldn't be cloneable nor assignable
            StateManager(const StateManager & obj) = delete;
            StateManager & operator=(const StateManager & obj) = delete;

            virtual ~StateManager()
            {
                critical_section_deinit(&stateManagerLock);
            }
    };

    // Initialize static member
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> stateTransMatrix>
    StateManager<StateId, Event, stateTransMatrix> *StateManager<StateId, Event, stateTransMatrix>::instance{nullptr};

} // namespace StateMachine
