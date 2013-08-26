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

#ifndef _CREMATCH_H_
#define _CREMATCH_H_

#include <vector>

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

class RematchButton
{
public:
    RematchButton();
    bool RematchClicked(int mouse_x, int mouse_y);

    bool Initialize(GLuint program);
    void Cleanup();
    void OnRender(GLuint program, glm::mat4 projection_matrix, glm::mat4 view_matrix);

private:
    GLuint texture;

    GLuint index_buffer;
    GLuint vertex_buffer;
    GLuint uv_buffer;

    glm::mat4 model_matrix;
    GLuint mvp_uniform;
    GLuint texture_sampler_uniform;
    GLuint alpha_color_uniform;
};

#endif
