#include <iostream>
#include "Board.h"
#include "Setting.h"
#include "Resource_manager.h"

void Board::Init()
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
 * The board is of repeated blank cell. The pieces are in a sprite that has image for a blank, a cell with red and a cell with yellow piece.
 */
void Board::Render()
{
	// Iterate over columns and rows and render play sprites and board overlay
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// Calculate coords for the board layout
			int x = col * Setting::grid_sprite_width;
			int y = row  * Setting::grid_sprite_height;

			// Position sprites in board layout and render sprite with current clipping rectangle
			Resource_manager::GetImage("cell_sprite")->SetPosition(x, y);
			Resource_manager::GetImage("cell_sprite")->Render(&sprite_clips[static_cast<int>(cells[col][row].current_sprite)]);

			// Position board overlay image in board layout and render
			Resource_manager::GetImage("grid_overlay")->SetPosition(x, y);
			Resource_manager::GetImage("grid_overlay")->Render();
		}
	}
}

/**
* Clears board by turning all to blank
*/
void Board::Clear()
{
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// Set the cell sprite to sprite_blank
			cells[col][row].current_sprite = Sprites::blank;
			cells[col][row].played_by = Players::nobody;
		}
	}
}

bool Board::CheckForDraw()
{
	// Iterate over columns and rows
	for (int col = 0; col < Setting::grid_columns; col++) {
		for (int row = 0; row < Setting::grid_rows; row++) {

			// If any of the cells have a sprite_blank clipping displayed there can be no draw
			if (cells[col][row].played_by == Players::nobody) {
				return false;
			}
		}
	}
	return true;
}

bool Board::CheckWin(int col, int row, Players player)
{
	if (CheckForRowWin(row, player)) {
		return true;
	}
	else if (CheckForColumnWin(col, player)) {
		return true;
	}
	else if (CheckForForwardsDiagonalWin(col, row, player)) {
		return true;
	}
	else if (CheckForBackwardsDiagonalWin(col, row, player)) {
		return true;
	}

	return false;
}

bool Board::CheckForRowWin(int row, Players player)
{
	int consecutive_matches = 0;

	// Iterate over columns in a specified row.
	for (int i = 0; i < Setting::grid_columns; i++) {

		// Count consecutive matches
		if (cells[i][row].played_by == player) {
			consecutive_matches++;
		}
		else {
			consecutive_matches = 0;
		}

		// If there are enough consecutive matches return true for a win
		if (consecutive_matches == Setting::win_count) {
			return true;
		}
	}

	return false;
}

bool Board::CheckForColumnWin(int col, Players player)
{
	int consecutive_matches = 0;

	// Iterate over rows in a specified column.
	for (int i = 0; i < Setting::grid_rows; i++) {

		// Count consecutive matches
		if (cells[col][i].played_by == player) {
			consecutive_matches++;
		}
		else {
			consecutive_matches = 0;
		}

		// If there are enough consecutive matches return true for a win
		if (consecutive_matches == Setting::win_count) {
			return true;
		}
	}

	return false;
}

bool Board::CheckForForwardsDiagonalWin(int col, int row, Players player)
{
	int consecutive_matches = 0;
	// Setting::win_count * 2 to check both sides of this move
	for (int i = 0; i < Setting::win_count * 2; i++) {
		// Count from the bottom of the forward diagonal to the top. [/]
		int check_col = col + i - Setting::win_count;
		int check_row = row - i + Setting::win_count;

		// Only check when coords are valid
		if ((check_col >= 0 && check_col < Setting::grid_columns) && (check_row >= 0 && check_row < Setting::grid_rows)) {

			// Count consecutive matches
			if (cells[check_col][check_row].played_by == player) {
				consecutive_matches++;
			}
			else {
				consecutive_matches = 0;
			}

			// If there are enough consecutive matches return true for a win
			if (consecutive_matches == Setting::win_count) {
				return true;
			}
		}
	}

	return false;
}

bool Board::CheckForBackwardsDiagonalWin(int col, int row, Players player)
{
	int consecutive_matches = 0;
	// Setting::win_count * 2 to check both sides of this move
	for (int i = 0; i < Setting::win_count * 2; i++) {

		// Count from the top of the backwards diagonal to the bottom. [\]
		int check_col = col + i - Setting::win_count;
		int check_row = row + i - Setting::win_count;

		// Only check when coords are valid
		if ((check_col >= 0 && check_col < Setting::grid_columns) && (check_row >= 0 && check_row < Setting::grid_rows)) {

			// Count consecutive matches
			if (cells[check_col][check_row].played_by == player) {
				consecutive_matches++;
			}
			else {
				consecutive_matches = 0;
			}

			// If there are enough consecutive matches return true for a win
			if (consecutive_matches == Setting::win_count) {
				return true;
			}
		}
	}

	return false;
}

int Board::GetBottommostAvailableRowInColumn(int col)
{
	// Scan from bottom to top of given column for available row
	for (int i = Setting::grid_rows - 1; i >= 0; i--) {
		if (cells[col][i].played_by == Players::nobody) {
			return i;
		}
	}

	return -1;
}

Board::Players Board::GetPiece(int col, int row)
{
	// Check if the column and row indices are valid
	if (col >= 0 && col < Setting::grid_columns && row >= 0 && row < Setting::grid_rows) {
		return cells[col][row].played_by;
	}
	else {
		// Invalid indices, return player_nobody
		return Players::nobody;
	}
}

bool Board::DropPiece(int col, Players player)
{
	// Check if column index valid
	if (col >= 0 && col < Setting::grid_columns) {
		// Find the bottommost available row in the specified column
		int row = GetBottommostAvailableRowInColumn(col);
		if (row != -1) {
			// Set the player who owns the cell in the specified column and row
			cells[col][row].played_by = player;
			// True if a piece drops successfully
			return true;
		}
	}
	// Column is full/invalid, return false
	return false;
}