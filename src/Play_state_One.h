#pragma once
#include <SDL.h>
#include "State.h"
#include "Grid.h"

/**
 * State implementation for handling the game play state
 */
class Play_state_One : public State
{
public:
	void init() override;
	void handle_events(SDL_Event& event) override;
	void update() override;
	void render() override;
	void clean_up() override;
private:
	/// Instance of the Grid class
	Grid grid;

	/// The sprite colour to be played and be toggled throughout moves. Red goes first
	Grid::sprite_sheet sprite_to_play = grid.sprite_red;

	/// Holds Grid.player_red (1), Grid.player_yellow (2) or Grid.player_red + Grid.player_yellow (3) for a draw
	int win_type;

	/**
	 * Called by handle_events() when a mouse button is pressed and advances
	 * the game by trying to play a move or clearing grid or returning to Menu
	 */
	void advance_game();

	int get_ai_player_move();

	int minimax(int depth, bool is_maximizing);

	int get_human_player_move();

	/**
	 * Try to play a move in the given column and row
	 *
	 * @param col Grid column number to play move in
	 * @param row Grid row number to play move in
	 *
	 * @return True if a move could be made
	 */
	bool play_a_move(int col, int row);

	/**
	 * See if the grid contains a win or if all moves have been made
	 *
	 * @param col Grid column number to run check from
	 * @param row Grid row number to run check from
	 *
	 * @return True if a win or draw was found
	 */
	bool check_for_win_or_draw(int col, int row);

	/// X, Y and to Y values for red piece drop animation
	int red_piece_x;
	int red_piece_y;
	int red_piece_to_y;

	/// X, Y and to Y values for yellow piece drop animation
	int yellow_piece_x;
	int yellow_piece_y;
	int yellow_piece_to_y;

	/// Store the column and row of the last move
	int previous_play_col;
	int previous_play_row;

	/// Store the previous sprite (red or yellow) of the last move
	Grid::sprite_sheet previous_play_sprite;

	/// Speed of animation for dropping piece
	int drop_speed;

	/// Max depth AI
	int max_depth = 3;
	/**
	 * Animate the dropping piece
	 */
	void animate_drop_on_play();

	/**
	 * Check if the drop animation is playing
	 *
	 * @return true if the animation is still playing
	 */
	bool drop_animation_is_playing();
};
