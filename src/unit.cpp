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

#include "unit.h"

Unit::Unit()
{
    unitset = 0;
    type = UNIT_TYPE_NONE;
    state = UNIT_STATE_NONE;
}

const unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

const GLfloat vertices[] = {
    0, 0, 0,      // Top left
    32, 0, 0,     // Top right
    32, 32, 0,    // Bottom right
    0, 32, 0      // Bottom left
};

/**
 * Initializes units (X and O)
 *
 * @param program requires a program object that contains all of the shaders
 *
 * @return returns true or false, whether or not it successfully initialized
 */
bool Unit::Initialize(GLuint program)
{
    /***************************
        Load unitset & clips
     ***************************/
    unitset = Painter::LoadImage("gfx/units.png");
    if (unitset == 0) return false;

    /********************************
      Initialize Texture Coordinates
     ********************************/
    int texture_width = 64;
    int texture_height = 32;
    int width = 32;
    int height = 32;

    uvs.clear();
    // OpenGL uses a column-major order instead of row-major order
    for (int x = 0; x < 2; x++) {
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

void Unit::Cleanup()
{
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteTextures(1, &unitset);
}

/**
 * Unit's OnRender function, called every frame
 *
 * @param unit_list A list of units, which is a multi-dimensional array.
 * @param program requires a program object that contains all of the shaders
 * @param projection_matrix requires an orthographic projection matrix
 * @param view_matrix requires a view matrix, created by glm::lookAt
 *
 */
void Unit::OnRender(
        std::vector< std::vector<Unit> > &unit_list,
        GLuint program,
        glm::mat4 projection_matrix,
        glm::mat4 view_matrix)
{
    if (unitset == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No unitset loaded.");
        return;
    }

    glUseProgram(program);

    // Draw units (X and O)
    for (unsigned int y = 0; y < unit_list.size(); y++) {
        for (unsigned int x = 0; x < unit_list[y].size(); x++) {
            model_matrix = glm::scale((float)ZOOM_LEVEL, (float)ZOOM_LEVEL, 0.0f);
            model_matrix *= glm::translate((float)x * TILE_SIZE, (float)y * TILE_SIZE, 0.0f);

            glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;
            // Pass on model_view_projection matrix to shader
            glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp));

            // Use tileset texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, unitset);
            glUniform1i(texture_sampler_uniform, 0);

            // Pass on vertices to shader
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            // Pass on texture coordinates to shader
            int clip_id = unit_list[x][y].type;
            glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, uvs[clip_id].size() * sizeof(GLfloat), &uvs[clip_id].front());
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            
            if (unit_list[x][y].state == UNIT_STATE_PLACED) {
                // Pass on alpha color to shader
                glUniform4f(alpha_color_uniform, 1.0f, 1.0f, 1.0f, 1.0f);

                // Finally draw the vertices
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
            } else if (unit_list[x][y].state == UNIT_STATE_TRANSPARENT) {
                // Pass on alpha color to shader
                glUniform4f(alpha_color_uniform, 1.0f, 1.0f, 1.0f, 0.4f);

                // Finally draw the vertices
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
            }
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
    }
}
