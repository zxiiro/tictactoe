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
    #include <SDL_opengl.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_opengl.h>
#endif

#include "digits.h"
#include "global.h"
#include "painter.h"

class ScoreBoard
{
public:
    ScoreBoard();

    bool Initialize(GLuint program);
    void Cleanup();
    void OnRender(GLuint program, glm::mat4 projection_matrix, glm::mat4 view_matrix);

    void AddPointPlayer1();
    void AddPointPlayer2();

private:
    GLuint               scoreboardset;

    GLuint               index_buffer;
    GLuint               vertex_buffer;
    GLuint               uv_buffer;

    glm::mat4            model_matrix;
    GLuint               mvp_uniform;
    GLuint               texture_sampler_uniform;
    GLuint               alpha_color_uniform;

    Digits               digits;
    int                  player1_score;
    int                  player2_score;

    // Offsets for where to draw each player's scoreboard
    static const int    PLAYER1_X_OFFSET = 7;
    static const int    PLAYER1_Y_OFFSET = 34;
    static const int    PLAYER2_X_OFFSET = 21;
    static const int    PLAYER2_Y_OFFSET = 70;

    std::queue<int> GetDigits(int number);
};

#endif
