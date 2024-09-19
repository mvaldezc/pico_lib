#pragma once
#include <memory>
#include "pico/critical_section.h"
#include "pico/mutex.h"
#include "lock_guard.hpp"
#include "state.hpp"
#include "state_factory.hpp"
#include <atomic>

namespace StateMachine {

    // State transition matrix data type
    template <typename StateId, typename Event>
    using StateTransMatrix = StateId (*)(StateId, Event);

    // State forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class State;

    // StateFactory forward declaration
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class StateFactory;

    // equivalent to static mutex declaration + mutex_init()
    auto_init_mutex(createStateManagerMutex);
    auto_init_mutex(stateTransitionMutex);

    /**
     * @class StateManager
     * @brief Manages the machine state transitions.
     * StateManager is defined as a leaky singleton and provides a thread/ISR-safe API for 
     * triggering machine state transitions.
     * 
     * @details
     * It implements a static mutex to avoid race conditions during singleton creation.
     * And a critical_section for concurrent access of its non-reentrant methods.
     * NOTE: For template classes, all member function defenitions must be in the header file. 
     * 
     * @tparam StateId Enum class representing the possible states.
     * @tparam Event Enum class representing the events that trigger state transitions.
     * @tparam FSM_STM State transition matrix function pointer.
     * 
     */
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    class StateManager
    {   
        protected:
            using State_ = State<StateId, Event, FSM_STM>;
            StateManager(StateId sId) : state(nullptr), currentStateId(sId)
            {
                critical_section_init(&stateManagerLock);
            }

            static StateManager * instance;
            std::unique_ptr<State_> state;
            critical_section_t stateManagerLock;
            StateId currentStateId;
            std::atomic_bool stateChanged = false;

            // Transition to a new state.
            void stateTransition(std::unique_ptr<State_> && newState)
            {
                lock_guard<mutex_t> mutexWrapper(stateTransitionMutex);
                state = std::move(newState);
            }

        public:
            // Singleton instance getter.
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

            // Handle event in current state.
            void handleEvent(Event && event)
            {
                critical_section_enter_blocking(&stateManagerLock);
                // Update the current state based on the event using the state transition matrix
                currentStateId = FSM_STM(currentStateId, event);
                critical_section_exit(&stateManagerLock);

                stateChanged = true;
            }

            // Monitor state changes, enter, run and exit states. To be called in a loop.
            void run()
            {
                // If state has changed
                if (stateChanged)
                {
                    // Exit current state
                    state->onExit();
                    // Create new state
                    auto newState = StateFactory<StateId, Event, FSM_STM>::createState(currentStateId);
                    // Transition to new state
                    stateTransition(std::move(newState));
                    // Enter new state
                    state->onEnter();
                    stateChanged = false;
                }

                // Run current state
                state->run();
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
    template <typename StateId, typename Event, StateTransMatrix<StateId, Event> FSM_STM>
    StateManager<StateId, Event, FSM_STM> *StateManager<StateId, Event, FSM_STM>::instance{nullptr};

} // namespace StateMachine
