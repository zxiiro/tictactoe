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

#ifndef _CPAINTER_H_
#define _CPAINTER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Painter {
public:
    Painter();

    // Tools for painting to the renderer
    static SDL_Texture* LoadImage(SDL_Renderer* renderer, const char* file);
    static bool DrawImage(SDL_Renderer* renderer, SDL_Texture* image, SDL_Rect* position, SDL_Rect* clip);
};

#endif
