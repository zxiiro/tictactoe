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

#include <SDL2/SDL.h>
 
int main(int argc, char** argv){
 
    // Start SDL2
    SDL_Init(SDL_INIT_EVERYTHING);
 
    // Create a Window in the middle of the screen
    SDL_Window *window = NULL;
    window = SDL_CreateWindow("Hello World!",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x and y
            640, 480, // Width and Height
            SDL_WINDOW_SHOWN);
 
    // Delay so that we can see the window appear
    SDL_Delay(2000);
 
    // Cleanup and Quit
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}

