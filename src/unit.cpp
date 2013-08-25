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
    unitset = NULL;
    unit_clips = new SDL_Rect[2];

    type = UNIT_TYPE_NONE;
    state = UNIT_STATE_NONE;
}

bool Unit::Initialize(SDL_Renderer* renderer)
{
    /***************************
        Load unitset & clips
     ***************************/
    unitset = Painter::LoadImage(renderer, "gfx/units.png");
    if (unitset == NULL) return false;

    for (int i = 0; i < 2; i++) {
        unit_clips[i].x = i * 32;
        unit_clips[i].y = 0;
        unit_clips[i].w = 32;
        unit_clips[i].h = 32;
    }

    return true;
}

void Unit::Cleanup()
{
    SDL_DestroyTexture(unitset);
}

/**
 * Unit's OnRender function, called every frame
 *
 * @param renderer Renderer to draw on
 * @param unit_list A list of units, which is a multi-dimensional array.
 *
 */
void Unit::OnRender(SDL_Renderer* renderer, std::vector< std::vector<Unit> > &unit_list)
{
    if (unitset == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "No unitset loaded.");
        return;
    }

    // Draw units (X and O)
    for (unsigned int y = 0; y < unit_list.size(); y++) {
        for (unsigned int x = 0; x < unit_list[y].size(); x++) {
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
