#pragma once
#include "state_manager.hpp"
#include "state_transition_matrix.hpp"
#include "fsm_state_factory.hpp"

namespace StateMachine {
	namespace RobotArm {

		class FSMStateManager : public StateManager<StateId, Event, stateTransMatrix>
		{
			public:
				FSMStateManager() : StateManager(StateId::Init) {}
				
				static StateManager * getInstance()
				{
					lock_guard<mutex_t> lock(createStateManagerMutex);
					if (instance == nullptr)
					{
						instance = new FSMStateManager();
					}
					return instance;
				}
		};

	} // namespace RobotArm
} // namespace StateMachine
