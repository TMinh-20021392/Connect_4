#pragma once
#include <string>

/**
 * Contains common setting variables for the game
 */
struct Setting
{
	/// Name that appears in the application titlebar
	const static char* game_name; 

	/// Path to the icon that appears in the application titlebar
	const static char* icon_path;

	/// Path to images relative to the source files, must end with _
	static const std::string image_path;

	/// Path to sounds relative to the source files, must end with _
	static const std::string sound_path;

	/// Set first element to -1 to disable colour keying, otherwise set to a colour such as { 0, 0xFF, 0xFF }
	static const int color_key[];

	/// Set a background colour for the renderer
	static const int background_color[];

	/// Set to false for peaceful development
	static const bool play_sounds = true;

	/// Changing the scale factor adjusts the size of the images and window
	static const int scale_factor = 8;

	/// Width of the clipped sprite sheet image used for the grid
	static const int grid_sprite_width = 16;

	/// Height of the clipped sprite sheet image used for the grid
	static const int grid_sprite_height = 16;

	static const int grid_columns = 7;

	static const int grid_rows = 6;

	/// The win count of consecutive pieces in a row, column or diagonal
	static const int win_count = 4;

	/// Width of the application window
	static const int window_width = grid_columns * grid_sprite_width;

	/// Height of the application window
	static const int window_height = grid_rows * grid_sprite_height;

	/// Width of the playing area without the scaling factor applied
	static const int actual_window_width = window_width * scale_factor;

	/// Height of the playing area without the scaling factor applied
	static const int actual_window_height = window_height * scale_factor;
};