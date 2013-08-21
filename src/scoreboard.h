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

#ifndef _CSCOREBOARD_H_
#define _CSCOREBOARD_H_

#include <SDL2/SDL.h>

#include "global.h"
#include "painter.h"

class ScoreBoard
{
public:
    static ScoreBoard    GameScoreBoard;
    SDL_Texture*    scoreboardset;

    ScoreBoard();

    bool Initialize();

    void OnRender(SDL_Renderer* renderer);
};

#endif
