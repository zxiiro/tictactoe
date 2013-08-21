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

Unit::Unit() {
    unitset = NULL;
    unit_clips = new SDL_Rect[2];

    unit_id = UNIT_TYPE_NONE;
}

bool Unit::Initialize() {
    /**********************
       Set Current Player
     **********************/
    currentPlayer = 0;

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
    for (int i = 0; i < 9; i++) {
        Unit tmp_unit;
        unit_list.push_back(tmp_unit);
    }

    return true;
}

void Unit::OnRender(SDL_Renderer* renderer) {
    if (unitset == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No unitset loaded.");
        return;
    }

    // Draw units (X and O)
    for (int i = 0; i < 9; i++) {
        // Check if the units in the list need to actually be drawn
        if (unit_list[i].unit_id != Unit::UNIT_TYPE_NONE) {
            int x = (i % 3);
            int y = (i / 3);

            SDL_Rect unit_pos;
            unit_pos.x = x * TILE_SIZE * ZOOM_LEVEL;
            unit_pos.y = y * TILE_SIZE * ZOOM_LEVEL;
            unit_pos.w = TILE_SIZE * ZOOM_LEVEL;
            unit_pos.h = TILE_SIZE * ZOOM_LEVEL;

            int unit_clip_id = unit_list[i].unit_id;
            Painter::DrawImage(renderer, unitset, &unit_pos, &unit_clips[unit_clip_id]);
        }
    }
}

void Unit::SetCell(int id)
{
    if (id < 0 || id >= 9) return;

    // Check if the cell in the unit list is NONE, otherwise it will overwrite existing units
    if (unit_list[id].unit_id == Unit::UNIT_TYPE_NONE) {
        if (currentPlayer == 0) {
            unit_list[id].unit_id = Unit::UNIT_TYPE_X;
            currentPlayer = 1;
        } else if (currentPlayer == 1) {
            unit_list[id].unit_id = Unit::UNIT_TYPE_O;
            currentPlayer = 0;
        }
    }
}