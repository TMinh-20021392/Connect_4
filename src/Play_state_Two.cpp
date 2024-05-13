#include "Play_state_Two.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Menu_state.h"

void Play_state_Two::Init()
{
	Resource_manager::LoadImage("cell_sprite");
	Resource_manager::LoadImage("grid_overlay");

	Resource_manager::LoadImage("red_piece")->SetPositionHidden();
	Resource_manager::LoadImage("yellow_piece")->SetPositionHidden();

	Resource_manager::LoadImage("red_wins")->SetPositionCenter();
	Resource_manager::LoadImage("yellow_wins")->SetPositionCenter();
	Resource_manager::LoadImage("draw")->SetPositionCenter();

	Resource_manager::LoadSound("valid_move");
	Resource_manager::LoadSound("invalid_move");
	Resource_manager::LoadSound("win");
	Resource_manager::LoadSound("draw");

	board.Init();

	// Get current position of the red piece
	red_piece_x = Resource_manager::LoadImage("red_piece")->GetPositionX();
	red_piece_y = Resource_manager::LoadImage("red_piece")->GetPositionY();

	// Get current position of the yellow piece
	yellow_piece_x = Resource_manager::LoadImage("yellow_piece")->GetPositionX();
	yellow_piece_y = Resource_manager::LoadImage("yellow_piece")->GetPositionY();

	// Store previous move
	previous_play_col = -1;
	previous_play_row = -1;
	previous_play_sprite = Board::Sprites::red;

	// Must divide 16 or it wont fall rightly on Y axis during animations causing glitches
	drop_speed = 8;
}

void Play_state_Two::Clean()
{
	Resource_manager::UnloadImage("cell_sprite");
	Resource_manager::UnloadImage("grid_overlay");

	Resource_manager::UnloadImage("red_piece");
	Resource_manager::UnloadImage("yellow_piece");

	Resource_manager::UnloadImage("red_wins");
	Resource_manager::UnloadImage("yellow_wins");
	Resource_manager::UnloadImage("draw");

	Resource_manager::UnloadSound("valid_move");
	Resource_manager::UnloadSound("invalid_move");
	Resource_manager::UnloadSound("win");
	Resource_manager::UnloadSound("draw");
}

void Play_state_Two::HandleEvent(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		AdvanceGame();
	}
}

void Play_state_Two::Update()
{
	AnimateDroppingPiece();
}

void Play_state_Two::Render()
{
	// Render yellow and red pieces now so they appear under board overlay that is rendered next
	Resource_manager::GetImage("red_piece")->Render();
	Resource_manager::GetImage("yellow_piece")->Render();

	board.Render();

	// Return if drop animation hasn't finished yet
	if (IsDropAnimationPlaying()) {
		return;
	}

	// Depending on win or draw conditions, display a win or draw message overlay image
	if (win_type == 1) {
		Resource_manager::GetImage("red_wins")->Render();
		Resource_manager::GetSound("win")->PlaySoundOnce();

	}
	else if (win_type == 2) {
		Resource_manager::GetImage("yellow_wins")->Render();
		Resource_manager::GetSound("win")->PlaySoundOnce();
	}
	else if (win_type == 3) {
		Resource_manager::GetImage("draw")->Render();
		Resource_manager::GetSound("draw")->PlaySoundOnce();
	}
}

void Play_state_Two::AdvanceGame() {

	// Don't allow a player to advance the game before drop animation has finished
	if (IsDropAnimationPlaying()) {
		return;
	}

	// If no winner
	if (!win_type) {

		// Get mouse coords and normalize to column
		int mouse_x;
		int mouse_y;
		Game::GetMousePosition(&mouse_x, &mouse_y);
		int col = mouse_x / (Setting::window_width  / Setting::grid_columns);
		// Get the row that the play can be dropped to
		int row = board.GetBottommostAvailableRowInColumn(col);

		// Play a move
		if (Play(col, row)) {
			CheckForGameEnd(col, row);
		}
		else {
			Resource_manager::GetSound("invalid_move")->PlaySound();
		}

	}

	// Reset the game if there's a winner or draw
	else {
		win_type = 0;
		sprite_to_play = Board::Sprites::red;
		board.Clear();
		State_manager::SetState(new Menu_state());
	}
}

bool Play_state_Two::Play(int col, int row) {

	// Since we will reuse an animation piece so we update the previous play before moving the animation piece. 
	// Before running this code, check "previous_play_col" and "previous_play_row" to prevent running first move where no previous play existed.
	if (previous_play_col >= 0 && previous_play_row >= 0) {
		board.cells[previous_play_col][previous_play_row].current_sprite = previous_play_sprite;
	}

	// If there is a row space in the column to play
	if (row >= 0) {

		// Store previous play
		previous_play_col = col;
		previous_play_row = row;
		previous_play_sprite = sprite_to_play;

		if (sprite_to_play == Board::Sprites::red) {
			// Toggle next sprite
			sprite_to_play = Board::Sprites::yellow;
			
			// Set X axis value of red piece so the animation will start and define how far to drop
			red_piece_x = col * Setting::grid_sprite_width;
			red_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by red
			board.cells[col][row].played_by = Board::Players::red;
		}
		else {
			// Toggle next sprite
			sprite_to_play = Board::Sprites::red;

			// Set X axis value of yellow piece so animation will start and define how far to drop
			yellow_piece_x = col * Setting::grid_sprite_width;
			yellow_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by yellow
			board.cells[col][row].played_by = Board::Players::yellow;
		}

		return true;
	}

	return false;
}

bool Play_state_Two::CheckForGameEnd(int col, int row) {
	// Check to see if the last move caused a win for red
	if (board.CheckWin(col, row, Board::Players::red)) {
		win_type = 1;
		return true;
	}

	// Check to see if the last move caused a win for yellow
	else if (board.CheckWin(col, row, Board::Players::yellow)) {
		win_type = 2;
		return true;
	}

	// Check to see if the last move caused a draw
	else if (board.CheckForDraw()) {
		win_type = 3;
		return true;
	}

	return false;
}

void Play_state_Two::AnimateDroppingPiece() {

	// Hidden position is -width for X and -height for Y for init image. 
	// Play() set X axis value of red or yellow piece, check if animation needs to start

	// Drop animation - advance red piece on Y axis
	if (red_piece_x != -Setting::grid_sprite_width) {
		red_piece_y += drop_speed;
		Resource_manager::GetImage("red_piece")->SetPosition(red_piece_x, red_piece_y);
	}

	// Check if we need to stop red animation by checking current Y axis value against red_piece_to_y set by Play()
	if (red_piece_y == red_piece_to_y) {
		red_piece_x = -Setting::grid_sprite_width;
		red_piece_y = -Setting::grid_sprite_height;

		Resource_manager::GetSound("valid_move")->PlaySound();
	}

	// Drop animation - advance yellow piece on Y axis
	if (yellow_piece_x != -Setting::grid_sprite_width) {
		yellow_piece_y += drop_speed;
		Resource_manager::GetImage("yellow_piece")->SetPosition(yellow_piece_x, yellow_piece_y);
	}

	// Check if we need to stop yellow animation by checking current Y axis value against yellow_piece_to_y set by Play()
	if (yellow_piece_y == yellow_piece_to_y) {
		yellow_piece_x = -Setting::grid_sprite_width;
		yellow_piece_y = -Setting::grid_sprite_height;

		Resource_manager::GetSound("valid_move")->PlaySound();
	}
}

bool Play_state_Two::IsDropAnimationPlaying() {
	// True if red or yellow piece is currently animating
	if (red_piece_y != -Setting::grid_sprite_height
		|| yellow_piece_y != -Setting::grid_sprite_height) {
		return true;
	}
	return false;
}