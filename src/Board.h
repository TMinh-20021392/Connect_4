#pragma once
#include <SDL.h>
#include <array>
#include "Setting.h"

/**
 * Rendering and logic for game's board and cells.
 */
class Board
{
public:
    /// Player names stored as an enum class
    enum class Players { nobody, red, yellow };

    /// Sprite names stored as an enum class
    enum class Sprites { blank, red, yellow };

    struct Cell {
        /// Location of a sprite
        SDL_Point position;

        /// Current part of a sprite sheet to display
        Sprites current_sprite = Sprites::blank;

        /// Flag check if this cell has been played
        Players played_by = Players::nobody;
    };

    /// An array of cells that makes the board
    std::array<std::array<Cell, Setting::grid_rows>, Setting::grid_columns> cells;

    /**
     * Init the board
     */
    void Init();

    /**
     * Draw a board
     */
    void Render();

    /**
     * Clear the board
     */
    void Clear();

    /**
     * Check if player won with this move
     */
    bool CheckWin(int col, int row, Players player);

    /**
     * Check if neither player has won the game, but the game has ended in a draw.
     */
    bool CheckForDraw();

    /**
     * Find the bottommost available row for play in a given column
     */
    int GetBottommostAvailableRowInColumn(int col);

    /**
     * Get the piece at a specific column and row.
     */
    Players GetPiece(int col, int row);

    /**
     * Drop a piece into a specific column for a given player.
     * @return True if the piece successfully dropped
     */
    bool DropPiece(int col, Players player);

private:
    /// Contains coords of clipping rectangles for the cell sprite sheet
    std::array<SDL_Rect, 3> sprite_clips;

    bool CheckForRowWin(int row, Players player);
    bool CheckForColumnWin(int col, Players player);
    bool CheckForForwardsDiagonalWin(int col, int row, Players player);
    bool CheckForBackwardsDiagonalWin(int col, int row, Players player);
};
