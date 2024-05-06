#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "State.h"

class Menu_state : public State
{
public:
    void init();
    void clean_up();

    void handle_events(SDL_Event& event);
    void update();
    void render();

private:
    bool within_box(int x, int y, int box_position_x, int box_position_y, int box_width, int box_height);
};

#endif
