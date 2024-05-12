#pragma once
#include <SDL.h>
#include "State.h"
#include "Board.h"

/**
 * State implementation for handling the game play state
 */
class Play_state_One : public State
{
public:
	void Init() override;
	void HandleEvent(SDL_Event& event) override;
	void Update() override;
	void Render() override;
	void Clean() override;
private:
	/// Instance of the Board class
	Board board;

	/// The sprite colour to be played and be toggled throughout moves. Red goes first
	Board::Sprites sprite_to_play = Board::Sprites::red;

	/// Holds red (1), yellow (2) or draw (3)
	int win_type;

	/**
	 * Called by HandleEvent() when a mouse button is pressed and advances game by trying to play a move/clearing board/returning to Menu
	 */
	void AdvanceGame();

	int GetAIMove();

	int Minimax(int depth, bool isMaximizingPlayer);

	bool CheckForDraw();

	int EvaluateBoard();

	/**
	 * Try to play a move
	 *
	 * @return True if a move could be made
	 */
	bool Play(int col, int row);

	/**
	 * See if the board contains a win or if all moves have been made
	 *
	 * @return True if a win or draw was found
	 */
	bool CheckForGameEnd(int col, int row);

	/// X, Y and to Y values for red piece drop animation
	int red_piece_x;
	int red_piece_y;
	int red_piece_to_y;

	/// X, Y and to Y values for yellow piece drop animation
	int yellow_piece_x;
	int yellow_piece_y;
	int yellow_piece_to_y;

	/// Store the column and row of last move
	int previous_play_col;
	int previous_play_row;

	/// Store the previous sprite (red or yellow) of the last move
	Board::Sprites previous_play_sprite;

	/// Animation speed for dropping piece
	int drop_speed;

	/// Max depth AI
	int max_depth = 3;
	/**
	 * Animate the dropping piece
	 */
	void AnimateDroppingPiece();

	/**
	 * Check if the drop animation is playing
	 *
	 * @return true if the animation is still playing
	 */
	bool IsDropAnimationPlaying();
};
