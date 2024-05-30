#pragma once
#include <SDL.h>

/**
 * Core of the game app: window, event, rendering, game loop and helper functions
 */
class Game
{
public:
	~Game();

	/**
 	 * Get instance of SDL_Window
	 */
	static SDL_Window* GetWindow();

	/**
	 * Get instance of SDL_Renderer
	 */
	static SDL_Renderer* GetRenderer();

	/**
	 * SDL init
	 *
	 * @param title App window title
	 * @return True if init succeed
	 */
	bool Init(const char* title, int window_width, int window_height) const;

	/**
	 * Game loop until Stop() is called
	 */
	void Run();

	/**
	 * Stops game loop
	 */
	void Stop();

	/**
	 * X and Y axis positions of mouse cursor divided by scale factor
	 */
	static void GetMousePosition(int* mouse_x, int* mouse_y);

private:
	// Flag check if game should stop running
	bool running = true;

	SDL_Event event;

	static SDL_Window* window;

	static SDL_Renderer* renderer;
};