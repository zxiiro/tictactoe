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

#include "rematchbutton.h"

RematchButton::RematchButton()
{
    texture = 0;
}

const unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

const GLfloat vertices[] = {
    0, 0, 0,      // Top left
    96, 0, 0,     // Top right
    96, 64, 0,    // Bottom right
    0, 64, 0      // Bottom left
};

const GLfloat uvs[] = {
    0, 0,         // Top left
    1, 0,         // Top right
    1, 1,         // Bottom right
    0, 1          // Bottom left
};

/**
 * Initializes rematch pop up button
 *
 * @param program requires a program object that contains all of the shaders
 *
 * @return returns true or false, whether or not it successfully initialized
 */
bool RematchButton::Initialize(GLuint program)
{
    /***************************
        Load tileset & clips
     ***************************/
    texture = Painter::LoadImage("gfx/popup.png");
    if (texture == 0) return false;

    /*******************************
          Create buffer objects
     ******************************/
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    /*****************************************
       Get uniform location from the shaders
     *****************************************/
    mvp_uniform = glGetUniformLocation(program, "mvp");
    texture_sampler_uniform = glGetUniformLocation(program, "textureSampler");
    alpha_color_uniform = glGetUniformLocation(program, "alphaColor");

    // Create identity matrix
    model_matrix = glm::mat4(1.0f);

    return true;
}

void RematchButton::Cleanup()
{
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteTextures(1, &texture);
}

/**
 * Render the rematch popup button
 *
 * @param program requires a program object that contains all of the shaders
 * @param projection_matrix requires an orthographic projection matrix
 * @param view_matrix requires a view matrix, created by glm::lookAt
 *
 */
void RematchButton::OnRender(GLuint program, glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    if (texture == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No tileset loaded.");
        return;
    }

    glUseProgram(program);

    float x = (WWIDTH - (96 * ZOOM_LEVEL)) / (2 * ZOOM_LEVEL);
    float y = (WHEIGHT - (64 * ZOOM_LEVEL)) / (2 * ZOOM_LEVEL);

    model_matrix = glm::scale((float)ZOOM_LEVEL, (float)ZOOM_LEVEL, 0.0f);
    model_matrix *= glm::translate(x, y, 0.0f);
    
    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;
    // Pass on model_view_projection matrix to shader
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp));

    // Pass on alpha color to shader
    glUniform4f(alpha_color_uniform, 1.0f, 1.0f, 1.0f, 1.0f);

    // Use texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texture_sampler_uniform, 0);

    // Pass on vertices to shader
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass on texture coordinates to shader
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Finally draw the vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/*
 * Initiates a rematch if the user clicks rematch button.
 *
 * @param mouse_x the position of the mouse in x coordinates, relative to the window.
 * @param mouse_y the position of the mouse in y coordinates, relative to the window.
 *
 * @return a boolean will be returned whether or not the rematch button has actually been clicked
 */
bool RematchButton::RematchClicked(int mouse_x, int mouse_y)
{
    int popup_pos_x = (WWIDTH - 96 * ZOOM_LEVEL) / 2;
    int popup_pos_y = (WHEIGHT - 64 * ZOOM_LEVEL) / 2;

    if ((mouse_x > popup_pos_x) &&
        (mouse_x < popup_pos_x + 96 * ZOOM_LEVEL) &&
        (mouse_y > popup_pos_y) &&
        (mouse_y < popup_pos_y + 64 * ZOOM_LEVEL))
    {
        return true;
    }

    return false;
}
