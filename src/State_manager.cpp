#include "State_manager.h"

std::vector<State*> State_manager::states;

void State_manager::SetState(State* state)
{
	if (!states.empty()) {

		// Call Clean() on state to free a state's memory
		states.back()->Clean();

		// Delete instance since it was created with new
		delete states.back();
		states.back() = nullptr;

		// Remove unneeded entry from vector
		states.pop_back();
	}

	// Store the new state and call its init()
	states.push_back(state);
	states.back()->Init();
}

State* State_manager::GetState() {
	return states.back();
}