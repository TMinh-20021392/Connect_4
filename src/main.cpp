#include <iostream>
#include <string>
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Menu_state.h"

int main(int argc, char* args[])
{
	Game game;

	if (!game.Init(Setting::game_name, Setting::window_width, Setting::window_height)) {
		std::cout << "Error. Exiting." << std::endl;
	}

	State_manager::SetState(new Menu_state());

	game.Run();
	return 0;
}