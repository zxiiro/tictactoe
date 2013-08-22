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

#ifndef _CUNIT_H_
#define _CUNIT_H_

#include <vector>

#include <SDL2/SDL.h>

#include "global.h"
#include "painter.h"

class Unit
{
public:
    static Unit GameUnits;
    SDL_Texture*    unitset;

    Unit();

    bool Initialize();
    void OnRender(SDL_Renderer* renderer);
    void SetCell(int id);
    void SetTransparentCell(int id);

private:
    int currentPlayer;
    int lastHover;

    enum State {
        UNIT_STATE_PLACED,
        UNIT_STATE_TRANSPARENT,
        UNIT_STATE_NONE
    };

    enum Type {
        UNIT_TYPE_X,
        UNIT_TYPE_O 
    };

    State state;
    Type type;

    SDL_Rect* unit_clips;
    std::vector<Unit> unit_list;
};

#endif
