#pragma once
#include <string>

/**
 * Contains common setting variables for the game
 */
struct Setting
{
	/// App title
	const static char* game_name; 

	/// App icon path
	const static char* icon_path;

	/// Path to images relative to the source files, must end with /
	static const std::string image_path;

	/// Path to sounds relative to the source files, must end with /
	static const std::string sound_path;

	/// Set first element to -1 to disable colour keying, otherwise set to a colour such as { 0, 0xFF, 0xFF }
	static const int color_key[];

	/// Set a background colour for the renderer
	static const int background_color[];

	/// Set to false for peaceful development
	static const bool play_sounds = true;

	/// Changing the scale factor adjusts the size of images and window
	static const int scale_factor = 8;

	/// Width of clipped sprite sheet image used for the grid
	static const int grid_sprite_width = 16;

	/// Height of clipped sprite sheet image used for the grid
	static const int grid_sprite_height = 16;

	static const int grid_columns = 7;

	static const int grid_rows = 6;

	/// The win count of consecutive pieces
	static const int win_count = 4;

	/// App window width
	static const int window_width = grid_columns * grid_sprite_width;

	/// App window height
	static const int window_height = grid_rows * grid_sprite_height;

	/// Game area width without the scaling factor
	static const int actual_window_width = window_width * scale_factor;

	/// Game area height without the scaling factor
	static const int actual_window_height = window_height * scale_factor;
};