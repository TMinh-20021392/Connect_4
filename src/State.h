#pragma once
#include <SDL.h>

/**
 * Abstract class for state interface
 */
class State
{
public:
    /**
     * Called when a state is loaded to init memory for a state
     */
    virtual void Init() = 0;

    /**
     * Called first in game loop to handle game events(button presses, mouse clicks, window resize etc)
     */
    virtual void HandleEvent(SDL_Event& event) = 0;

    /**
     * Called second in game loop to update game items, scores etc, handle game logic.
     */
    virtual void Update() = 0;

    /**
     * Called third in game loop to render items to window
     */
    virtual void Render() = 0;

    /**
     * Called when a state is unloaded to free memory for a state
     */
    virtual void Clean() = 0;
};