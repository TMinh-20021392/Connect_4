#include "Menu_state.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Play_state_Two.h"
#include "Play_state_One.h"
void Menu_state::init()
{
    const int ButtonWidth = 40;
    const int ButtonHeight = 10;
    const int TitleWidth = 100;
    const int TitleHeight = Setting::window_height / 3;
    Resource_manager::load_image("background")->set_position_with_size(0,0,Setting::window_width, Setting::window_height);
    Resource_manager::load_image("Title")->set_position_with_size((Setting::window_width / 2) - (TitleWidth / 2), 10, 100, TitleHeight);
    Resource_manager::load_image("ONEPLAYERButton")->set_position_with_size((Setting::window_width / 2) - (ButtonWidth / 2), Setting::window_height / 2, ButtonWidth, ButtonHeight);
    Resource_manager::load_image("TWOPLAYERButton")->set_position_with_size((Setting::window_width / 2) - (ButtonWidth / 2), Setting::window_height / 2 + ButtonHeight + 5, ButtonWidth, ButtonHeight);
}

void Menu_state::clean_up()
{
    Resource_manager::unload_image("background");
    Resource_manager::unload_image("Title");
    Resource_manager::unload_image("ONEPLAYERButton");
    Resource_manager::unload_image("TWOPLAYERButton");
}

void Menu_state::handle_events(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        // Get mouse coords
        int mouse_x;
        int mouse_y;
        Game::get_mouse_position(&mouse_x, &mouse_y);

        // Check if click falls within box coords
        if (within_box(mouse_x, mouse_y, 36, 48, 40, 10)) {
            State_manager::set_state(new Play_state_One());
        }
        else if (within_box(mouse_x, mouse_y, 36, 63, 40, 10)) {
            State_manager::set_state(new Play_state_Two());
        }
    }
}

void Menu_state::update()
{
    // Add any necessary updates here
}

void Menu_state::render()
{
    Resource_manager::get_image("background")->render();
    Resource_manager::get_image("Title")->render();
    Resource_manager::get_image("ONEPLAYERButton")->render();
    Resource_manager::get_image("TWOPLAYERButton")->render();
}

bool Menu_state::within_box(int x, int y, int box_position_x, int box_position_y, int box_width, int box_height)
{
    // Check four conditions to determine if X and Y are inside the box
    if (x >= box_position_x // Not too far left
        && x < box_position_x + box_width // Not too far right
        && y >= box_position_y // Not too far up
        && y < box_position_y + box_height) // Not too far down
    {
        return true;
    }

    return false;
}
