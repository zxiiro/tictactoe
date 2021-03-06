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

#ifndef _CBOARD_H_
#define _CBOARD_H_

#include <vector>

#ifdef ANDROID
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "global.h"
#include "painter.h"
#include "tile.h"
#include "unit.h"

class Board
{
private:
    SDL_Rect*           tile_clips;
    std::vector<Tile>   tile_list;

public:
    SDL_Texture*    tileset;

    Board();

    bool Initialize(SDL_Renderer* renderer);
    void Cleanup();
    void OnRender(SDL_Renderer* renderer);
};

#endif
