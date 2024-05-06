#pragma once
#include <SDL.h>

/**
 * Abstract class for state interface
 *
 * Games typically have many states that often need to perform the same functions.
 */
class State
{
public:
    /**
     * Called when a state is loaded to initialise memory for a state
     */
    virtual void init() = 0;

    /**
     * Called first in game loop to handle game events such as button presses, mouse clicks, window resize.
     */
    virtual void handle_events(SDL_Event& event) = 0;

    /**
     * Called second in game loop to update game items, scores etc. Game logic will be handled here.
     */
    virtual void update() = 0;

    /**
     * Called third in game loop to render items to the game window. This
     * should be the only area that items are rendered within.
     */
    virtual void render() = 0;

    /**
     * Called when a state is unloaded to free memory for a state
     */
    virtual void clean_up() = 0;
};