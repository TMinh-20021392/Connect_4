#include "Menu_state.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Play_state_Two.h"
#include "Play_state_One.h"
void Menu_state::Init()
{
    const int ButtonWidth = 40;
    const int ButtonHeight = 10;
    const int TitleWidth = 100;
    const int TitleHeight = Setting::window_height / 3;
    Resource_manager::LoadImage("background")->SetPositionWithSize(0,0,Setting::window_width, Setting::window_height);
    Resource_manager::LoadImage("Title")->SetPositionWithSize((Setting::window_width / 2) - (TitleWidth / 2), 10, 100, TitleHeight);
    Resource_manager::LoadImage("ONEPLAYERButton")->SetPositionWithSize((Setting::window_width / 2) - (ButtonWidth / 2), Setting::window_height / 2, ButtonWidth, ButtonHeight);
    Resource_manager::LoadImage("TWOPLAYERButton")->SetPositionWithSize((Setting::window_width / 2) - (ButtonWidth / 2), Setting::window_height / 2 + ButtonHeight + 5, ButtonWidth, ButtonHeight);
}

void Menu_state::Clean()
{
    Resource_manager::UnloadImage("background");
    Resource_manager::UnloadImage("Title");
    Resource_manager::UnloadImage("ONEPLAYERButton");
    Resource_manager::UnloadImage("TWOPLAYERButton");
}

void Menu_state::HandleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        // Get mouse coords
        int mouse_x;
        int mouse_y;
        Game::GetMousePosition(&mouse_x, &mouse_y);

        // Check if click falls within box coords
        if (WithinBox(mouse_x, mouse_y, 36, 48, 40, 10)) {
            State_manager::SetState(new Play_state_One());
        }
        else if (WithinBox(mouse_x, mouse_y, 36, 63, 40, 10)) {
            State_manager::SetState(new Play_state_Two());
        }
    }
}

void Menu_state::Update()
{
    
}

void Menu_state::Render()
{
    Resource_manager::GetImage("background")->Render();
    Resource_manager::GetImage("Title")->Render();
    Resource_manager::GetImage("ONEPLAYERButton")->Render();
    Resource_manager::GetImage("TWOPLAYERButton")->Render();
}

bool Menu_state::WithinBox(int x, int y, int box_x, int box_y, int box_w, int box_h)
{
    if (x >= box_x // Not too far left
        && x < box_x + box_w // Not too far right
        && y >= box_y // Not too far up
        && y < box_y + box_h) // Not too far down
    {
        return true;
    }

    return false;
}
