#pragma once
#include <vector>
#include "State.h"

/**
 * Manages the current game state
 */
class State_manager
{
public:
	/**
	 * Set the current state to run
	 *
	 * Called from the Game::Run() loop
	 */
	static void SetState(State* state);

	/**
	 * Gets the current running state
	 */
	static State* GetState();

private:
	/// Stores pointers to instances of a State
	static std::vector<State*> states;
};