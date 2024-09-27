/***********************************************************************
 * @file	:	fsm_state_manager.hpp
 * @brief 	:	RobotArm StateManager class
 *              Library for RobotArm finite state machine functionality.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include "state_manager.hpp"
#include "state_transition_matrix.hpp"
#include "fsm_state_factory.hpp"

namespace StateMachine {
namespace RobotArm {
	
	/**
	 * @class FSMStateManager
	 * @brief StateManager for the RobotArm finite state machine.
	 * 
	 * @tparam StateId Enum class representing the possible states.
	 * @tparam Event Enum class representing the events that trigger state transitions.
	 * @tparam stateTransMatrix State transition matrix function pointer.
	 */
	class FSMStateManager : public StateManager<StateId, Event, stateTransMatrix>
	{
		public:
			FSMStateManager() : StateManager(StateId::Init) {}
			
			/**
			 * @brief Singleton instance getter.
			 * 
			 * @return Pointer to the StateManager instance.
			 */
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
