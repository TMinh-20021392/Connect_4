#include "Play_state_One.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Menu_state.h"
#include <random>

void Play_state_One::init()
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

	// Must divide nicely into 16 or it won't fall on the right parts of the Y axis during animations causing glitches
	drop_speed = 8;
}

void Play_state_One::clean_up()
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

void Play_state_One::handle_events(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		advance_game();
	}
}

void Play_state_One::update()
{
	animate_drop_on_play();
}

void Play_state_One::render()
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

void Play_state_One::advance_game() {
	// Don't allow a player to advance the game before drop animation has finished
	if (drop_animation_is_playing()) {
		return;
	}
	if (!win_type) {
		// Human player's turn
		int col = get_human_player_move();
		int row = grid.get_bottommost_available_row_in_column(col);

		if (play_a_move(col, row)) {
			if (check_for_win_or_draw(col, row)) {
				return; // Exit early if the game is won or drawn
			}
			// Reset the game if there's a winner or draw after human player's move
			if (win_type) {
				win_type = 0;
				sprite_to_play = grid.sprite_red;
				grid.clear();
				State_manager::set_state(new Menu_state());
				return;
			}
		}
		else {
			Resource_manager::get_sound("invalid_move")->play();
		}
	}

	// AI player's turn
	if (!win_type) {
		int ai_col = get_ai_player_move();
		int ai_row = grid.get_bottommost_available_row_in_column(ai_col);

		if (play_a_move(ai_col, ai_row)) {
			if (check_for_win_or_draw(ai_col, ai_row)) {
				return; // Exit early if the game is won or drawn
			}
		}
		else {
			// Handle invalid move for AI player
		}
	}

	// Reset the game if there's a winner or draw
	if (win_type) {
		win_type = 0;
		sprite_to_play = grid.sprite_red;
		grid.clear();
		State_manager::set_state(new Menu_state());
	}
}

int Play_state_One::get_ai_player_move() {
	int best_move = -1;
	int best_score = std::numeric_limits<int>::min();

	for (int col = 0; col < Setting::grid_columns; ++col) {
		int row = grid.get_bottommost_available_row_in_column(col);
		if (row >= 0) {
			// Make the move
			grid.cell[col][row].played_by = grid.player_yellow;

			// Evaluate the move using minimax
			int score = minimax(0, false);

			// Undo the move
			grid.cell[col][row].played_by = grid.player_nobody;

			// Update best move if necessary
			if (score > best_score) {
				best_score = score;
				best_move = col;
			}
		}
	}

	return best_move;
}
int Play_state_One::minimax(int depth, bool is_maximizing) {
	// Check for terminal conditions
	auto result = static_cast<int>(grid.check_win());
	if (result != 0) {
		if (result == grid.player_yellow) {
			return 10 - depth;
		}
		else if (result == grid.player_red) {
			return depth - 10;
		}
		else {
			return 0; // Draw
		}
	}
	else if (depth >= max_depth) {
		return 0; // Depth limit reached, evaluate current state
	}

	if (is_maximizing) {
		int best_score = std::numeric_limits<int>::min();
		for (int col = 0; col < Setting::grid_columns; ++col) {
			int row = grid.get_bottommost_available_row_in_column(col);
			if (row >= 0) {
				// Make the move
				grid.cell[col][row].played_by = grid.player_yellow;

				// Recursively call minimax for the opponent
				int score = minimax(depth + 1, false);

				// Undo the move
				grid.cell[col][row].played_by = grid.player_nobody;

				// Update best score
				best_score = std::max(best_score, score);
			}
		}
		return best_score;
	}
	else {
		int best_score = std::numeric_limits<int>::max();
		for (int col = 0; col < Setting::grid_columns; ++col) {
			int row = grid.get_bottommost_available_row_in_column(col);
			if (row >= 0) {
				// Make the move
				grid.cell[col][row].played_by = grid.player_red;

				// Recursively call minimax for the opponent
				int score = minimax(depth + 1, true);

				// Undo the move
				grid.cell[col][row].played_by = grid.player_nobody;

				// Update best score
				best_score = std::min(best_score, score);
			}
		}
		return best_score;
	}
}


int Play_state_One::get_human_player_move() {
	int mouse_x;
	int mouse_y;
	Game::get_mouse_position(&mouse_x, &mouse_y);
	return mouse_x / (Setting::window_width / Setting::grid_columns);
}

bool Play_state_One::play_a_move(int col, int row) {

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

bool Play_state_One::check_for_win_or_draw(int col, int row) {
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

void Play_state_One::animate_drop_on_play() {

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

bool Play_state_One::drop_animation_is_playing() {
	// True if red or yellow piece is currently animating
	if (red_piece_y != -Setting::grid_sprite_height
		|| yellow_piece_y != -Setting::grid_sprite_height) {
		return true;
	}
	return false;
}
