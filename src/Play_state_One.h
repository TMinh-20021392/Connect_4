#pragma once
#include <SDL.h>
#include "State.h"
#include "Board.h"

/**
 * State implementation for handling game play state
 */
class Play_state_One : public State
{
public:
	Play_state_One();
	explicit Play_state_One(bool ai_first);
	void Init() override;
	void HandleEvent(SDL_Event& event) override;
	void Update() override;
	void Render() override;
	void Clean() override;
private:
	Board board;

	/// The sprite colour to be played and be toggled throughout moves. Red goes first
	Board::Sprites sprite_to_play = Board::Sprites::red;

	/// Holds red (1), yellow (2), draw (3)
	int win_type;

	/**
	 * Called by HandleEvent() when a mouse button is pressed and advances game by trying to play a move/clearing board/returning to Menu
	 */
	void AdvanceGame();

	int GetAIMove();

	int Minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);

	bool CheckForDraw();

	int EvaluateBoard();

	/**
	 * Try to play a move
	 *
	 * @return True if a move could be made
	 */
	bool Play(int col, int row);

	/**
	 * True if a win or draw was found
	 */
	bool CheckForGameEnd(int col, int row);

	/// X, Y and to Y values for red drop animation
	int red_piece_x;
	int red_piece_y;
	int red_piece_to_y;

	/// X, Y and to Y values for yellow drop animation
	int yellow_piece_x;
	int yellow_piece_y;
	int yellow_piece_to_y;

	/// Store column and row of last move
	int previous_play_col = 0;
	int previous_play_row = 0;

	/// Store previous sprite of last move
	Board::Sprites previous_play_sprite;

	/// Animation speed for dropping piece
	int drop_speed;

	/// Max depth AI
	int max_depth = 3;

	bool ai_first = true;
	bool firsttime = true;
	bool win_announced = false;
	/**
	 * Animate the dropping piece
	 */
	void AnimateDroppingPiece();

	/**
	 * Check if drop animation is playing
	 */
	bool IsDropAnimationPlaying();

	bool WithinBox(int x, int y, int box_x, int box_y, int box_w, int box_h);

	bool IsHumanTurn();
};
