#include <iostream>
#include "Grid.h"
#include "Setting.h"
#include "Resource_manager.h"

void Grid::init()
{
	// Setup sprite clipping rects
	for (int i = 0; i < 3; i++) {
		sprite_clips[i].x = 0;
		sprite_clips[i].y = i * Setting::grid_sprite_height;
		sprite_clips[i].w = Setting::grid_sprite_width;
		sprite_clips[i].h = Setting::grid_sprite_height;
	}
}

/**
 * The grid is of repeated blank cell. The pieces are in a sprite that has image for a blank, a cell with red and a cell with yellow piece.
 */
void Grid::render()
{
	// Iterate over columns and rows and render play sprites and grid overlay
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// Calculate coords for the grid layout
			int x = col * Setting::grid_sprite_width;
			int y = row  * Setting::grid_sprite_height;

			// Position sprites in grid layout and render sprite with current clipping rectangle
			Resource_manager::get_image("cell_sprite")->set_position(x, y);
			Resource_manager::get_image("cell_sprite")->render(&sprite_clips[cell[col][row].current_sprite]);

			// Position grid overlay image in grid layout and render
			Resource_manager::get_image("grid_overlay")->set_position(x, y);
			Resource_manager::get_image("grid_overlay")->render();
		}
	}
}

/**
* Clears grid by turning all to blank
*/
void Grid::clear()
{
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// Set the cell sprite to sprite_blank
			cell[col][row].current_sprite = sprite_blank;
			cell[col][row].played_by = player_nobody;
		}
	}
}

const char* Grid::get_player_name(int sprite)
{
	// Define an array player names
	static const char* player_names[] = { "Red", "Yellow" };

	// Return a player name
	return player_names[sprite -1];

}

bool Grid::check_for_draw()
{
	// Iterate over columns and rows
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// If any of the cells have a sprite_blank clipping displayed there can be no draw
			if (cell[col][row].played_by == player_nobody) {
				return false;
			}
		}
	}

	std::cout << "Draw for both players" << std::endl;
	return true;
}

bool Grid::check_for_win(int col, int row, players player)
{
	if (check_for_row_win(col, row, player)) {
		return true;
	}
	else if (check_for_column_win(col, row, player)) {
		return true;
	}
	else if (check_for_forwards_diagonal_win(col, row, player)) {
		return true;
	}
	else if (check_for_backwards_diagonal_win(col, row, player)) {
		return true;
	}

	return false;
}

bool Grid::check_for_row_win(int col, int row, players player)
{
	// How many times a particular sprite clipping is matched
	int consecutive_matches = 0;

	// Iterate over columns in a specified row. For example:
	// [X][X][X]
	// [ ][ ][ ]
	// [ ][ ][ ]
	for (int i = 0; i < Setting::grid_columns; i++) {

		// Count consecutive matches
		if (cell[i][row].played_by == player) {
			consecutive_matches++;
		}
		else {
			consecutive_matches = 0;
		}

		// If there are enough consecutive matches return true for a win
		if (consecutive_matches == Setting::win_count) {
			std::cout << "Row win for " << get_player_name(player) << " player" << std::endl;
			return true;
		}
	}

	return false;
}

bool Grid::check_for_column_win(int col, int row, players player)
{
	// How many times a particular sprite clipping is matched
	int consecutive_matches = 0;

	// Iterate over rows in a specified column. For example:
	// [X][ ][ ]
	// [X][ ][ ]
	// [X][ ][ ]
	for (int i = 0; i < Setting::grid_rows; i++) {

		// Count consecutive matches
		if (cell[col][i].played_by == player) {
			consecutive_matches++;
		}
		else {
			consecutive_matches = 0;
		}

		// If there are enough consecutive matches return true for a win
		if (consecutive_matches == Setting::win_count) {
			std::cout << "Column win for " << get_player_name(player) << " player" << std::endl;
			return true;
		}
	}

	return false;
}

bool Grid::check_for_forwards_diagonal_win(int col, int row, players player)
{
	// How many times a particular sprite clipping is matched
	int consecutive_matches = 0;

	// Check for Setting::win_count * 2 times for the reason explained below
	for (int i = 0; i < Setting::win_count * 2; i++) {

		// Count from the bottom of the forwards diagonal to the top. For 
		// example, on a 3x3 grid where the move made is marked by "4" the 
		// check will run in the direction of 1 to 6 starting outside of the
		// grid as shown below:
		// 
		//       COL 0  1  2
		//          [ ][ ][6] ROW 0
		//          [ ][5][ ] ROW 1
		//          [4][ ][ ] ROW 2
		//       [3]
		//    [2]
		// [1]
		//
		// If the move made is marked by "3". The search pattern would look
		// like the following:
		//
		//                         [6]
		//                      [5]
		//       COL 0  1  2 [4]
		//          [ ][ ][3] ROW 0
		//          [ ][2][ ] ROW 1
		//          [1][ ][ ] ROW 2
		//
		int check_col = col + i - Setting::win_count;
		int check_row = row - i + Setting::win_count;

		// Only check when coords are valid otherwise we get false positives
		if ((check_col >= 0 && check_col < Setting::grid_columns) && (check_row >= 0 && check_row < Setting::grid_rows)) {

			// Count consecutive matches
			if (cell[check_col][check_row].played_by == player) {
				consecutive_matches++;
			}
			else {
				consecutive_matches = 0;
			}

			// If there are enough consecutive matches return true for a win
			if (consecutive_matches == Setting::win_count) {
				std::cout << "Forwards diagonal (/) win for " << get_player_name(player) << " player" << std::endl;
				return true;
			}
		}
	}

	return false;
}

bool Grid::check_for_backwards_diagonal_win(int col, int row, players player)
{
	// How many times a particular sprite clipping is matched
	int consecutive_matches = 0;

	// Check for Setting::win_count * 2 times for the reason explained below
	for (int i = 0; i < Setting::win_count * 2; i++) {

		// Count from the top of the backwards diagonal to the bottom. For 
		// example, on a 3x3 grid where the move made is marked by "5" the 
		// check will run in the direction of 1 to 6 starting outside of the
		// grid as shown below:
		// 
		// [1]  
		//    [2]
		//       [3] 0  1  2 
		//          [4][ ][ ] ROW 0
		//          [ ][5][ ] ROW 1
		//          [ ][ ][6] ROW 2
		//
		// If the move made is marked by "3". The search pattern would look
		// like the following:
		//
		//  
		//
		//           0  1  2 
		//          [1][ ][ ] ROW 0
		//          [ ][2][ ] ROW 1
		//          [ ][ ][3] ROW 2
		//                   [4]
		//                      [5]
		//                         [6]
		//
		int check_col = col + i - Setting::win_count;
		int check_row = row + i - Setting::win_count;

		// Only check when coords are valid otherwise we get false positives
		if ((check_col >= 0 && check_col < Setting::grid_columns) && (check_row >= 0 && check_row < Setting::grid_rows)) {

			// Count consecutive matches
			if (cell[check_col][check_row].played_by == player) {
				consecutive_matches++;
			}
			else {
				consecutive_matches = 0;
			}

			// If there are enough consecutive matches return true for a win
			if (consecutive_matches == Setting::win_count) {
				std::cout << "Backwards diagonal (\\) win for " << get_player_name(player) << " player" << std::endl;
				return true;
			}
		}
	}

	return false;
}

int Grid::get_bottommost_available_row_in_column(int col)
{
	// Scan from bottom to top of given column for available row
	for (int i = Setting::grid_rows - 1; i >= 0; i--) {
		if (cell[col][i].played_by == player_nobody) {
			return i;
		}
	}

	return -1;
}

Grid::players Grid::get_piece(int col, int row)
{
	// Check if the column and row indices are valid
	if (col >= 0 && col < Setting::grid_columns && row >= 0 && row < Setting::grid_rows) {
		return cell[col][row].played_by;
	}
	else {
		// Invalid indices, return player_nobody (or handle error according to your needs)
		return player_nobody;
	}
}

bool Grid::drop_piece(int col, players player)
{
	// Check if the column index is valid
	if (col >= 0 && col < Setting::grid_columns) {
		// Find the bottommost available row in the specified column
		int row = get_bottommost_available_row_in_column(col);
		if (row != -1) {
			// Set the player who owns the cell in the specified column and row
			cell[col][row].played_by = player;
			// Optionally, you might want to update the sprite or current_sprite here

			// Return true to indicate that the piece was successfully dropped
			return true;
		}
	}
	// Column is full or invalid, return false to indicate failure to drop the piece
	return false;
}
Grid::players Grid::check_win() {
	for (int col = 0; col < Setting::grid_columns; ++col) {
		for (int row = 0; row < Setting::grid_rows; ++row) {
			// Check if the current cell is played by a player
			if (cell[col][row].played_by != player_nobody) {
				// Check for a win starting from this cell
				if (check_for_win(col, row, cell[col][row].played_by)) {
					// Return the player who won
					return cell[col][row].played_by;
				}
			}
		}
	}
	// No winner found
	return player_nobody;
}