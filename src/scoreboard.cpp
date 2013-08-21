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

#include "scoreboard.h"

ScoreBoard ScoreBoard::GameScoreBoard;

ScoreBoard::ScoreBoard()
{
    scoreboardset = NULL;
}

/**
 * Initializes the scoreboard
 */
bool ScoreBoard::Initialize()
{
    return true;
}

void ScoreBoard::OnRender(SDL_Renderer* renderer)
{
    SDL_Rect pos;
    pos.x = 3 * TILE_SIZE * ZOOM_LEVEL;
    pos.y = 0;
    pos.w = 64 * ZOOM_LEVEL;
    pos.h = 96 * ZOOM_LEVEL;

    Painter::DrawImage(renderer, scoreboardset, &pos, NULL);
}
