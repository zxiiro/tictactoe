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

Digits::Digits()
{
    digits = NULL;
    digits_clips = new SDL_Rect[10];
}

/**
 * Initializes the scoreboard
 */
bool Digits::Initialize(SDL_Renderer* renderer)
{
    digits = Painter::LoadImage(renderer, "gfx/numbers.png");
    if (digits == NULL) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load digits.");
        return false;
    }

    for (int i = 0; i < 10; i++) {
        digits_clips[i].x = i * 6;
        digits_clips[i].y = 0;
        digits_clips[i].w = 6;
        digits_clips[i].h = 9;
    }

    return true;
}
