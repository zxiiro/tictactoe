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

#include <queue>

#ifdef ANDROID
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "digits.h"
#include "global.h"
#include "painter.h"

class ScoreBoard
{
public:
    SDL_Texture*         scoreboardset;
    Digits               digits;
    int                  player1_score;
    int                  player2_score;

    ScoreBoard();

    bool Initialize(SDL_Renderer* renderer);
    void OnRender(SDL_Renderer* renderer);

    void AddPointPlayer1();
    void AddPointPlayer2();

private:
    // Offsets for where to draw each player's scoreboard
    static const int    PLAYER1_X_OFFSET = 7;
    static const int    PLAYER1_Y_OFFSET = 34;
    static const int    PLAYER2_X_OFFSET = 21;
    static const int    PLAYER2_Y_OFFSET = 70;

    std::queue<int> GetDigits(int number);
};

#endif
