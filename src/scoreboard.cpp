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
    scoreboardset = NULL;
}

/**
 * Initializes the scoreboard
 */
bool ScoreBoard::Initialize(SDL_Renderer* renderer)
{
    scoreboardset = Painter::LoadImage(renderer, "gfx/scoreboard.png");
    player1_score = 0;
    player2_score = 0;

    digits.Initialize(renderer);

    return true;
}

void ScoreBoard::Cleanup()
{
    digits.Cleanup();
    SDL_DestroyTexture(scoreboardset);
}

void ScoreBoard::OnRender(SDL_Renderer* renderer)
{
    /**********************
        Draw Scoreboard
     **********************/
    SDL_Rect pos;
    pos.x = 3 * TILE_SIZE * ZOOM_LEVEL;
    pos.y = 0;
    pos.w = 64 * ZOOM_LEVEL;
    pos.h = 96 * ZOOM_LEVEL;

    Painter::DrawImage(renderer, scoreboardset, &pos, NULL);

    /**************************
        Draw Score (Digits)
     **************************/
    // player 1 scoreboard
    std::queue<int> p1_digits = GetDigits(player1_score);
    for (int i = 5; i >= 0; i--) {  // Start with the least significant digit
        SDL_Rect p1_pos;
        p1_pos.x = (i * 6 * ZOOM_LEVEL) +              // Digit position starting from left
                   (3 * TILE_SIZE * ZOOM_LEVEL) +      // Game Board Offset
                   (PLAYER1_X_OFFSET * ZOOM_LEVEL);    // Player score offset
        p1_pos.y = (PLAYER1_Y_OFFSET * ZOOM_LEVEL);    // Number of pixels offset from 0
        p1_pos.w = 6 * ZOOM_LEVEL;
        p1_pos.h = 9 * ZOOM_LEVEL;

        if (!p1_digits.empty()) {
            int number = p1_digits.front();
            p1_digits.pop();
            Painter::DrawImage(renderer, digits.digits, &p1_pos, &digits.digits_clips[number]);
        }
        else break;
    }

    // player 2 scoreboard
    std::queue<int> p2_digits = GetDigits(player2_score);
    for (int i = 5; i >= 0; i--) {  // Start with the least significant digit
        SDL_Rect p2_pos;
        p2_pos.x = (i * 6 * ZOOM_LEVEL) +              // Digit position starting from left
                   (3 * TILE_SIZE * ZOOM_LEVEL) +      // Game Board Offset
                   (PLAYER2_X_OFFSET * ZOOM_LEVEL);    // Player score offset
        p2_pos.y = (PLAYER2_Y_OFFSET * ZOOM_LEVEL);    // Number of pixels offset from 0
        p2_pos.w = 6 * ZOOM_LEVEL;
        p2_pos.h = 9 * ZOOM_LEVEL;

        if (!p2_digits.empty()) {
            int number = p2_digits.front();
            p2_digits.pop();
            Painter::DrawImage(renderer, digits.digits, &p2_pos, &digits.digits_clips[number]);
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
