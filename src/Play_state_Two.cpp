#include "Play_state_Two.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Menu_state.h"

void Play_state_Two::init()
{
	Resource_manager::load_image("cell_sprite");
	Resource_manager::load_image("grid_overlay");

	Resource_manager::load_image("red_piece")->set_position_hidden();
	Resource_manager::load_image("yellow_piece")->set_position_hidden();

	Resource_manager::load_image("red_wins")->set_position_centered();
	Resource_manager::load_image("yellow_wins")->set_position_centered();
	Resource_manager::load_image("draw")->set_position_centered();

	Resource_manager::load_sound("valid_move");
	Resource_manager::load_sound("invalid_move");
	Resource_manager::load_sound("win");
	Resource_manager::load_sound("draw");

	grid.init();

	// Get the current position of the red piece for the drop animation
	red_piece_x = Resource_manager::load_image("red_piece")->get_position_x();
	red_piece_y = Resource_manager::load_image("red_piece")->get_position_y();

	// Get the current position of the yellow piece for the drop animation
	yellow_piece_x = Resource_manager::load_image("yellow_piece")->get_position_x();
	yellow_piece_y = Resource_manager::load_image("yellow_piece")->get_position_y();

	// Store previous move
	previous_play_col = -1;
	previous_play_row = -1;
	previous_play_sprite = grid.sprite_red;

	// Must divide nicely into 16 or it wont fall on the right parts of the Y axis during animations causing glitches
	drop_speed = 8;
}

void Play_state_Two::clean_up()
{
	Resource_manager::unload_image("cell_sprite");
	Resource_manager::unload_image("grid_overlay");

	Resource_manager::unload_image("red_piece");
	Resource_manager::unload_image("yellow_piece");

	Resource_manager::unload_image("red_wins");
	Resource_manager::unload_image("yellow_wins");
	Resource_manager::unload_image("draw");

	Resource_manager::unload_sound("valid_move");
	Resource_manager::unload_sound("invalid_move");
	Resource_manager::unload_sound("win");
	Resource_manager::unload_sound("draw");
}

void Play_state_Two::handle_events(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		advance_game();
	}
}

void Play_state_Two::update()
{
	animate_drop_on_play();
}

void Play_state_Two::render()
{
	// Render yellow and red pieces now so that they will appear under grid overlay that is rendered next
	Resource_manager::get_image("red_piece")->render();
	Resource_manager::get_image("yellow_piece")->render();

	grid.render();

	// Return if drop animation hasn't finished yet
	if (drop_animation_is_playing()) {
		return;
	}

	// Depending on win or draw conditions, display a win or draw message overlay image
	if (win_type == grid.player_red) {
		Resource_manager::get_image("red_wins")->render();
		Resource_manager::get_sound("win")->play_once_only();

	}
	else if (win_type == grid.player_yellow) {
		Resource_manager::get_image("yellow_wins")->render();
		Resource_manager::get_sound("win")->play_once_only();
	}
	else if (win_type == grid.player_red + grid.player_yellow) {
		Resource_manager::get_image("draw")->render();
		Resource_manager::get_sound("draw")->play_once_only();
	}
}

void Play_state_Two::advance_game() {

	// Don't allow a player to advance the game before drop animation has finished
	if (drop_animation_is_playing()) {
		return;
	}

	// If no winner
	if (!win_type) {

		// Get mouse coordinates and normalise to row and columns
		int mouse_x;
		int mouse_y;
		Game::get_mouse_position(&mouse_x, &mouse_y);
		int col = mouse_x / (Setting::window_width  / Setting::grid_columns);
		// Get the row that the play can be dropped to
		int row = grid.get_bottommost_available_row_in_column(col);

		// Play a move
		if (play_a_move(col, row)) {
			check_for_win_or_draw(col, row);
		}
		// Otherwise invalid move
		else {
			Resource_manager::get_sound("invalid_move")->play();
		}

	}

	// Else if there was a win or draw, reset the winner, make next sprite red and clear the grid
	else {
		win_type = 0;
		sprite_to_play = grid.sprite_red;
		grid.clear();
		State_manager::set_state(new Menu_state());
	}
}

bool Play_state_Two::play_a_move(int col, int row) {

	// Since we will reuse an animation piece so we update the previous play before moving the animation piece. 
	// Before running this code, "previous_play_col" and "previous_play_row" are checked to prevent running on first move when no previous play was made.
	if (previous_play_col >= 0 && previous_play_row >= 0) {
		grid.cell[previous_play_col][previous_play_row].current_sprite = previous_play_sprite;
	}

	// If there is a row space in the column for the play to be dropped to
	if (row >= 0) {

		// Store previous play
		previous_play_col = col;
		previous_play_row = row;
		previous_play_sprite = sprite_to_play;

		if (sprite_to_play == grid.sprite_red) {
			// Set the next sprite
			sprite_to_play = grid.sprite_yellow;
			
			// Set X axis value of red piece so the animation will start and define how far to drop
			red_piece_x = col * Setting::grid_sprite_width;
			red_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by red player
			grid.cell[col][row].played_by = grid.player_red;
		}
		else {
			// Set the next sprite
			sprite_to_play = grid.sprite_red;

			// Set X axis value of yellow piece so animation will start and define how far to drop
			yellow_piece_x = col * Setting::grid_sprite_width;
			yellow_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by yellow player
			grid.cell[col][row].played_by = grid.player_yellow;
		}

		return true;
	}

	return false;
}

bool Play_state_Two::check_for_win_or_draw(int col, int row) {
	// Check to see if the last move caused a win for red
	if (grid.check_for_win(col, row, grid.player_red)) {
		win_type = grid.player_red;
		return true;
	}

	// Check to see if the last move caused a win for yellow
	else if (grid.check_for_win(col, row, grid.player_yellow)) {
		win_type = grid.player_yellow;
		return true;
	}

	// Check to see if the last move caused a draw
	else if (grid.check_for_draw()) {
		win_type = grid.player_red + grid.player_yellow;
		return true;
	}

	return false;
}

void Play_state_Two::animate_drop_on_play() {

	// Hidden position is -width for X and -height for Y for init image. 
	// play_a_move() set X axis value of red or yellow piece, check if animation needs to start

	// Drop animation - advance red piece on Y axis
	if (red_piece_x != -Setting::grid_sprite_width) {
		red_piece_y += drop_speed;
		Resource_manager::get_image("red_piece")->set_position(red_piece_x, red_piece_y);
	}

	// Check if we need to stop red piece drop animation by checking current Y axis value against red_piece_to_y set by play_a_move()
	if (red_piece_y == red_piece_to_y) {
		red_piece_x = -Setting::grid_sprite_width;
		red_piece_y = -Setting::grid_sprite_height;

		Resource_manager::get_sound("valid_move")->play();
	}

	// Drop animation - advance yellow piece on Y axis
	if (yellow_piece_x != -Setting::grid_sprite_width) {
		yellow_piece_y += drop_speed;
		Resource_manager::get_image("yellow_piece")->set_position(yellow_piece_x, yellow_piece_y);
	}

	// Check if we need to stop yellow piece drop animation by checking current Y axis value against yellow_piece_to_y set by play_a_move()
	if (yellow_piece_y == yellow_piece_to_y) {
		yellow_piece_x = -Setting::grid_sprite_width;
		yellow_piece_y = -Setting::grid_sprite_height;

		Resource_manager::get_sound("valid_move")->play();
	}
}

bool Play_state_Two::drop_animation_is_playing() {
	// True if red or yellow piece is currently animating
	if (red_piece_y != -Setting::grid_sprite_height
		|| yellow_piece_y != -Setting::grid_sprite_height) {
		return true;
	}
	return false;
}