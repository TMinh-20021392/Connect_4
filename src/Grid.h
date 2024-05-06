#pragma once
#include <SDL.h>
#include "Setting.h"

/**
 * Rendering and logic for the game's grid and cells.
 */
class Grid
{
public:
	/// Player names stored as an enum for easy reference
	enum players { player_nobody, player_red, player_yellow };

	/// Names of sprites stored as an enum for easy reference
	enum sprite_sheet { sprite_blank, sprite_red, sprite_yellow };

	/// Holds data of a cell
	struct Cell {

		/// Location of a sprite
		SDL_Point position;

		/// Current part of a sprite sheet to display
		sprite_sheet current_sprite = sprite_blank;

		/// Flag to check if this cell has been played
		players played_by = player_nobody;
	};
	
	/// An array of cells that describes the grid
	Cell cell[Setting::grid_columns][Setting::grid_rows];

	/**
	 * Initialises the grid state
	 */
	void init();

	/**
	 * Draws a grid
	 */
	void render();

	/**
	 * Clears the grid of any current plays
	 */
	void clear();

	/**
	 * Check to see if a player has won the game
	 *
	 * @param col Grid column number to run check from
	 * @param row Grid row number to run check from
	 * @param player Name of player move to check cell for
	 * @return True if a win was found
	 */
	bool check_for_win(int col, int row, players player);
	Grid::players check_win();
	/**
	 * Check to see neither player has won the game but the game has ended.
	 *
	 * A draw will happen with neither player has won but no more moves can be played.
	 *
	 * @return True if a win was found
	 */
	bool check_for_draw();

	/**
     * Find the bottommost available row for play in a given column
	 *
	 * @param col Column to check in
	 * @return bottommost available row number
	 */
	int get_bottommost_available_row_in_column(int col);

	/**
	 * Get the piece at a specific column and row.
	 *
	 * @param col Column index
	 * @param row Row index
	 * @return Player enum representing the piece at the specified location
	 */
	players get_piece(int col, int row);

	/**
	 * Drop a piece into a specific column for a given player.
	 *
	 * @param col Column index where the piece should be dropped
	 * @param player Player enum representing the player who is dropping the piece
	 * @return True if the piece was successfully dropped, false otherwise (e.g., column full)
	 */
	bool drop_piece(int col, players player);
private:
	/// Contains coords of clipping rectangles for the cell sprite sheet
	SDL_Rect sprite_clips[3];

	/**
	 * Used by check_for_X_win functions when printing debug messages
	 *
	 * @param sprite Value from enum sprite_sheet
	 * @return Name of a player
	 */
	const char* get_player_name(int sprite);

	/**
	 * @see check_for_win() for documentation of these member functions
	 */
	bool check_for_row_win(int col, int row, players player);
	bool check_for_column_win(int col, int row, players player);
	bool check_for_forwards_diagonal_win(int col, int row, players player);
	bool check_for_backwards_diagonal_win(int col, int row, players player);
};