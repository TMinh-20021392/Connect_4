#include <iostream>
#include <string>
#include <SDL_mixer.h>
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"

// Initialise static data-members 
SDL_Window* Game::window = nullptr;
SDL_Renderer* Game::renderer = nullptr;

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	// Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}

SDL_Window* Game::GetWindow()
{
	return window;
}

SDL_Renderer* Game::GetRenderer()
{
	return renderer;
}

bool Game::Init(const char* title, int window_width, int window_height)
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create window
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width * Setting::scale_factor, window_height * Setting::scale_factor, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set titlebar icon
	if (Setting::icon_path) {
		SDL_Surface* icon = SDL_LoadBMP(Setting::icon_path);
		if (icon == nullptr) {
			std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			return false;
		}

		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set background colour
	SDL_SetRenderDrawColor(renderer, Setting::background_color[0], Setting::background_color[1], Setting::background_color[2], Setting::background_color[3]);

	// Initialise SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

void Game::Run()
{
	while (running)
	{
		// Loop until all pending events have left the queue 
		while (SDL_PollEvent(&event)) {

			// Let the state handle events
			State_manager::GetState()->HandleEvent(event);

			// Handle window quit event
			if (event.type == SDL_QUIT) {
				Game::Stop();
			}
		}

		// Update data of the state
		State_manager::GetState()->Update();

		// Clear screen
		SDL_RenderClear(renderer);

		// Render the state
		State_manager::GetState()->Render();

		// Update screen taking into account SDL v-sync. This stops the game loop from running too fast
		SDL_RenderPresent(renderer);

	}
}

void Game::Stop()
{
	running = false;
}

void Game::GetMousePosition(int* mouse_x, int* mouse_y)
{
	SDL_GetMouseState(mouse_x, mouse_y);
	*mouse_x = *mouse_x / Setting::scale_factor;
	*mouse_y = *mouse_y / Setting::scale_factor;
}