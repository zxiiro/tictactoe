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

#ifndef _CENGINE_H_
#define _CENGINE_H_

#ifdef ANDROID
    #include <SDL.h>
    #include <SDL_opengl.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_opengl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "global.h"
#include "painter.h"
#include "loadshader.h"
#include "board.h"
#include "unit.h"
#include "scoreboard.h"
#include "rematchbutton.h"

class Engine
{
private:
    bool            running;
    bool            match_inprogress;
    Board           gameboard;
    Unit            gameunits;
    ScoreBoard      scoreboard;
    RematchButton   rematch_popup;

    SDL_Window*     window;
    SDL_GLContext   gl_context;
    GLuint          program;
    GLuint          vertex_array_object;

    glm::mat4       projection_matrix;
    glm::mat4       view_matrix;

    int             last_hover_x;
    int             last_hover_y;
    int             current_player;
    int             move_count;
    int             winner;
    std::vector< std::vector<Unit> > unit_list;

    bool Initialize();
    void Cleanup();

    // Events
    void OnEvent(SDL_Event* event);
    void OnLoop();
    void OnRender();

    void OnMouseLeftButtonDown(int mouse_x, int mouse_y);
    void OnMouseMove(int mouse_x, int mouse_y);

    void OnWindowResize(int width, int height);

    // Tic Tac Toe Logic
    void ResetBoard();
    void Rematch();
    void CheckWinner(int x, int y, Unit::Type current_unit_type);
    void PlaceUnit(int mouse_x, int mouse_y);
    void HoverUnit(int mouse_x, int mouse_y);

    void OnWin();

public:
    Engine();

    int Execute();
};

#endif
