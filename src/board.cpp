/**********************************************************************

    Copyright (c) 2013 Thanh Ha

    This file is part of sdl-tictactoe.

    sdl-tictactoe is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sdl-tictactoe is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sdl-tictactoe.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************/

#include "board.h"

Board Board::GameBoard;

Board::Board()
{
    tileset = NULL;
    tile_clips = new SDL_Rect[2];
}

/**
 * Initializes a tic-tac-toe board
 *
 * The board type is a 3x3 board
 */
bool Board::Initialize()
{
    /*************************
        Load tileset clips
     *************************/

    for (int i = 0; i < 2; i++) { // Tic Tac Toe only has 2 clips
        tile_clips[i].x = i * 32;
        tile_clips[i].y = 0;
        tile_clips[i].w = 32;
        tile_clips[i].h = 32;
    }

    /*****************************
        Initialize tile vector
     *****************************/

    tile_list.clear();
    for (int y = 0; y < 3; y++) // 3x3 board (y axis)
    {
        for (int x = 0; x < 3; x++) // 3x3 board (x axis)
        {
            // Switch tile types every other tile
            // The offset is to swap the starting tile on every row
            int offset = y % 2;
            int tile_id = (x % 2 + offset) % 2;

            Tile tmp_tile;
            tmp_tile.tile_id = tile_id;
            tile_list.push_back(tmp_tile);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
                "Loaded Tile: %d:%d", tile_id, offset);
        }
    }

    return true;
}

void Board::OnRender(SDL_Renderer* renderer)
{
    if (tileset == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No tileset loaded.");
        return;
    }

    int tile_id = 0;
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            SDL_Rect tile_pos;
            tile_pos.x = x * TILE_SIZE * ZOOM_LEVEL;
            tile_pos.y = y * TILE_SIZE * ZOOM_LEVEL;
            tile_pos.w = TILE_SIZE * ZOOM_LEVEL;
            tile_pos.h = TILE_SIZE * ZOOM_LEVEL;

            int clip_id = tile_list[tile_id++].tile_id;
            Painter::DrawImage(renderer, tileset, &tile_pos, &tile_clips[clip_id]);
        }
    }
}
