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

ScoreBoard::ScoreBoard()
{
    scoreboardset = 0;
}

const unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

const GLfloat vertices[] = {
    0, 0, 0,      // Top left
    64, 0, 0,     // Top right
    64, 96, 0,    // Bottom right
    0, 96, 0      // Bottom left
};

const GLfloat uvs[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1
};

/**
 * Initializes the scoreboard
 *
 * @param program requires a program object that contains all of the shaders
 *
 * @return returns true or false, whether or not it successfully initialized
 */
bool ScoreBoard::Initialize(GLuint program)
{
    scoreboardset = Painter::LoadImage("gfx/scoreboard.png");
    if (scoreboardset == 0) return false;

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

    player1_score = 0;
    player2_score = 0;

    digits.Initialize(program);

    return true;
}

void ScoreBoard::Cleanup()
{
    digits.Cleanup();
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteTextures(1, &scoreboardset);
}

/**
 * Render the scoreboard
 *
 * @param program requires a program object that contains all of the shaders
 * @param projection_matrix requires an orthographic projection matrix
 * @param view_matrix requires a view matrix, created by glm::lookAt
 *
 */
void ScoreBoard::OnRender(GLuint program, glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    if (scoreboardset == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No tileset loaded.");
        return;
    }

    /****************************
           Draw Scorebaord
     ****************************/
    glUseProgram(program);

    model_matrix = glm::scale((float)ZOOM_LEVEL, (float)ZOOM_LEVEL, 0.0f);
    model_matrix *= glm::translate((float)3 * TILE_SIZE, 0.0f, 0.0f);

    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;
    // Pass on model_view_projection matrix to shader
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp));

    // Pass on alpha color to shader
    glUniform4f(alpha_color_uniform, 1.0f, 1.0f, 1.0f, 1.0f);

    // Use scoreboard texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scoreboardset);
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

    /**************************
        Draw Score (Digits)
     **************************/
    // player 1 scoreboard
    std::queue<int> p1_digits = GetDigits(player1_score);
    for (int i = 5; i >= 0; i--) {  // Start with the least significant digit

        float x = (i * 6) +              // Digit position starting from left
                  (3 * TILE_SIZE) +      // Game Board Offset
                  (PLAYER1_X_OFFSET);    // Player score offset
        float y = (PLAYER1_Y_OFFSET);    // Number of pixels offset from 0

        if (!p1_digits.empty()) {
            int number = p1_digits.front();
            p1_digits.pop();

            // Pass on texture coordinates to shader
            digits.OnRender(program, projection_matrix, view_matrix, x, y, number);
        }
        else break;
    }

    // player 2 scoreboard
    std::queue<int> p2_digits = GetDigits(player2_score);
    for (int i = 5; i >= 0; i--) {  // Start with the least significant digit

        float x = (i * 6) +               // Digit position starting from left
                  (3 * TILE_SIZE) +       // Game Board Offset
                  (PLAYER2_X_OFFSET);     // Player score offset
        float y = (PLAYER2_Y_OFFSET);     // Number of pixels offset from 0

        if (!p2_digits.empty()) {
            int number = p2_digits.front();
            p2_digits.pop();

            digits.OnRender(program, projection_matrix, view_matrix, x, y, number);
        }
        else break;
    }
}

/**
 * Increase Player 1's score by 1
 */
void ScoreBoard::AddPointPlayer1()
{
    player1_score++;
}

/**
 * Increase Player 2's score by 1
 */
void ScoreBoard::AddPointPlayer2()
{
    player2_score++;
}

/**
 * Get's the individual digits in a number as a queue
 *
 * @param number    number to extract digits from
 * @return          fifo queue containing digits in a number
 */
std::queue<int> ScoreBoard::GetDigits(int number)
{
    std::queue<int> digits_queue;

    if (number == 0)
    {
        digits_queue.push(0);
    }
    else
    {
        while (number != 0)
        {
          int last = number % 10;
          digits_queue.push(last);
          number = (number - last) / 10;
        }
    }

    return digits_queue;
}
