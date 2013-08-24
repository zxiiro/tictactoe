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

Unit Unit::GameUnits;

Unit::Unit()
{
    unitset = NULL;
    unit_clips = new SDL_Rect[2];

    type = UNIT_TYPE_NONE;
    state = UNIT_STATE_NONE;
    last_hover_x = 0;
    last_hover_y = 0;
}

bool Unit::Initialize()
{
    /**********************
       Set Current Player
     **********************/
    currentPlayer = UNIT_TYPE_X;

    /**********************
       Set Move Counter
     **********************/
    moveCount = 0;

    /*************************
        Load unitset clips
     *************************/
    for (int i = 0; i < 2; i++) {
        unit_clips[i].x = i * 32;
        unit_clips[i].y = 0;
        unit_clips[i].w = 32;
        unit_clips[i].h = 32;
    }

    /*****************************
        Initialize unit vector
     *****************************/
    unit_list.clear();
    for (int x = 0; x < 3; x++) {
        std::vector<Unit> tmp_unit_list;

        for (int y = 0; y < 3; y++) {
            Unit tmp_unit;
            tmp_unit_list.push_back(tmp_unit);
        }

        unit_list.push_back(tmp_unit_list);
    }

    return true;
}

void Unit::OnRender(SDL_Renderer* renderer)
{
    if (unitset == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No unitset loaded.");
        return;
    }

    // Draw units (X and O)
    for (unsigned int x = 0; x < unit_list.size(); x++) {
        for (unsigned int y = 0; y < unit_list[x].size(); y++) {
            SDL_Rect unit_pos;
            unit_pos.x = x * TILE_SIZE * ZOOM_LEVEL;
            unit_pos.y = y * TILE_SIZE * ZOOM_LEVEL;
            unit_pos.w = TILE_SIZE * ZOOM_LEVEL;
            unit_pos.h = TILE_SIZE * ZOOM_LEVEL;

            if (unit_list[x][y].state == UNIT_STATE_PLACED) {
                SDL_SetTextureAlphaMod(unitset, 255);

                int unit_clip_id = unit_list[x][y].type;
                Painter::DrawImage(renderer, unitset, &unit_pos, &unit_clips[unit_clip_id]);
            }
            else if (unit_list[x][y].state == UNIT_STATE_TRANSPARENT) {
                SDL_SetTextureAlphaMod(unitset, 100);

                int unit_clip_id = unit_list[x][y].type;
                Painter::DrawImage(renderer, unitset, &unit_pos, &unit_clips[unit_clip_id]);
            }
        }
    }
}

void Unit::SetCell(int x, int y)
{
    if (x < 0 || x >= 3) return;
    if (y < 0 || y >= 3) return;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "Player %i moves to %ix%i", currentPlayer + 1, x, y);

    Type currentUnitType = UNIT_TYPE_NONE;

    // Check if the cell in the unit list hasn't been placed, otherwise it will overwrite existing units
    if (unit_list[x][y].state != UNIT_STATE_PLACED) {
        moveCount++;
        if (currentPlayer == UNIT_TYPE_X) {
            currentUnitType = UNIT_TYPE_X;
            unit_list[x][y].type = UNIT_TYPE_X;
            unit_list[x][y].state = UNIT_STATE_PLACED;
            currentPlayer = UNIT_TYPE_O;
        }
        else if (currentPlayer == UNIT_TYPE_O) {
            currentUnitType = UNIT_TYPE_O;
            unit_list[x][y].type = UNIT_TYPE_O;
            unit_list[x][y].state = UNIT_STATE_PLACED;
            currentPlayer = UNIT_TYPE_X;
        }
    }

    // Check columns
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[x][i].type != currentUnitType)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[x][i].type == UNIT_TYPE_X) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Player %i won!", UNIT_TYPE_X + 1);
            }
            else if (unit_list[x][i].type == UNIT_TYPE_O) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Player %i won!", UNIT_TYPE_O + 1);
            }
        }
    }

    // Check rows
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[i][y].type != currentUnitType)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[i][y].type == UNIT_TYPE_X) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Player %i won!", UNIT_TYPE_X + 1);
            }
            else if (unit_list[i][y].type == UNIT_TYPE_O) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Player %i won!", UNIT_TYPE_O + 1);
            }
        }
    }

    // Check diagonals
    if (x == y) {
        for (unsigned int i = 0; i < unit_list.size(); i++) {
            // If there's any units that aren't placed, then exit loop
            if (unit_list[i][i].type != currentUnitType)
                break;

            // If all units are placed on the board then there must be a clear winner
            if (i == unit_list.size() - 1) {
                // Determine winner (X or O)
                if (unit_list[i][i].type == UNIT_TYPE_X) {
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                            "Player %i won!", UNIT_TYPE_X + 1);
                }
                else if (unit_list[i][i].type == UNIT_TYPE_O) {
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                            "Player %i won!", UNIT_TYPE_O + 1);
                }
            }
        }
    }

    // Check Anti Diagonals
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[i][(unit_list.size() - 1) - i].type != currentUnitType)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[i][(unit_list.size() - 1) - i].type == UNIT_TYPE_X) {
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                            "Player %i won!", UNIT_TYPE_X + 1);
            }
            else if (unit_list[i][(unit_list.size() - 1) - i].type == UNIT_TYPE_O) {
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                            "Player %i won!", UNIT_TYPE_O + 1);
            }
        }
    }

    // Check for Draw
    if (moveCount >= 9) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "It's a draw!");
    }
}

void Unit::SetTransparentCell(int x, int y)
{
    if (x < 0 || x >= 3) return;
    if (y < 0 || y >= 3) return;

    // Unset the previously hovered cell (remove transparent unit)
    if (unit_list[last_hover_x][last_hover_y].state == UNIT_STATE_TRANSPARENT) {
        unit_list[last_hover_x][last_hover_y].state = UNIT_STATE_NONE;
        unit_list[last_hover_x][last_hover_y].type = UNIT_TYPE_NONE;
    }

    // Check if the cell in the unit list is NONE, otherwise it will overwrite existing units
    if (unit_list[x][y].state == UNIT_STATE_NONE) {
        if (currentPlayer == UNIT_TYPE_X) {
            unit_list[x][y].type = UNIT_TYPE_X;
            unit_list[x][y].state = UNIT_STATE_TRANSPARENT;
        }
        else if (currentPlayer == UNIT_TYPE_O) {
            unit_list[x][y].type = UNIT_TYPE_O;
            unit_list[x][y].state = UNIT_STATE_TRANSPARENT;
        }

        // Finally the new last hover is now the cell we set as transparent
        last_hover_x = x;
        last_hover_y = y;
    }
}
