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
    texture = 0;
}

const unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

const GLfloat vertices[] = {
    0, 0, 0,     // Top left
    6, 0, 0,     // Top right
    6, 9, 0,     // Bottom right
    0, 9, 0      // Bottom left
};

/**
 * Initializes the digits
 *
 * @param program  requires a program object that contains all of the shaders
 *
 * @return returns true or flase, whether or not it successfully initialized
 */
bool Digits::Initialize(GLuint program)
{
    texture = Painter::LoadImage("gfx/numbers.png");
    if (texture == 0) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load digits.");
        return false;
    }

    /********************************
      Initialize Texture Coordinates
     ********************************/
    int texture_width = 60;
    int texture_height = 9;
    int width = 6;
    int height = 9;

    uvs.clear();
    for (int x = 0; x < 10; x++) {
        std::vector<GLfloat> tmp_uvs;

        for (int y = 0; y < 1; y++) {
            // Top left
            tmp_uvs.push_back(x * width / (float)texture_width);
            tmp_uvs.push_back(y / (float)texture_height);

            // Top right
            tmp_uvs.push_back((x * width + width) / (float)texture_width);
            tmp_uvs.push_back(y / (float)texture_height);

            // Bottom right
            tmp_uvs.push_back((x * width + width) / (float)texture_width);
            tmp_uvs.push_back((y + height) / (float)texture_height);

            // Bottom left
            tmp_uvs.push_back(x * width / (float)texture_width);
            tmp_uvs.push_back((y + height) / (float)texture_height);
        }

        uvs.push_back(tmp_uvs);
    }

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
    glBufferData(GL_ARRAY_BUFFER, uvs[0].size() * sizeof(GLfloat), &uvs[0].front(), GL_STREAM_DRAW);

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

void Digits::Cleanup()
{
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteTextures(1, &texture);
}

/**
 * Renders a digit at a given x and y, with the id
 *
 * @param program requires a program object that contains all of the shaders
 * @param projection_matrix requires an orthorgraphic projection matrix
 * @param view_matrix requires a view matrix, created by glm::lookAt
 * @param x the horizontal coordinate of where you want to draw the digit
 * @param y the vertical coordinate of where you want to draw teh digit
 * @param id the number/id of what you want to draw, ranges from 0 to 9
 *
 */
void Digits::OnRender(GLuint program, glm::mat4 projection_matrix, glm::mat4 view_matrix, float x, float y, int id)
{
    if (texture == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "No digit texture loaded.");
        return;
    }

    glUseProgram(program);
    model_matrix = glm::scale((float)ZOOM_LEVEL, (float)ZOOM_LEVEL, 0.0f);
    model_matrix *= glm::translate(x, y, 0.0f);

    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;
    // Pass on model_view_projection matrix to shader
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp));

    // Pass on alpha color to shader
    glUniform4f(alpha_color_uniform, 1.0f, 1.0f, 1.0f, 1.0f);

    // Use digit texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texture_sampler_uniform, 0);

    // Pass on vertices to shader
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass on texture coordinates to shader
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, uvs[id].size() * sizeof(GLfloat), &uvs[id].front());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Finally draw the vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
