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

#include "painter.h"

/**
 * Loads an image into memory
 *
 * @param renderer  renderer to use
 * @param file      filename of image to load
 *
 * @return          returns the loaded image
 */
SDL_Texture* Painter::LoadImage(SDL_Renderer* renderer, const char* file)
{
    SDL_Surface* tmp_image = NULL;
    SDL_Texture* image = NULL;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Loading image... %s", file);
    tmp_image = IMG_Load(file);
    if (tmp_image == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load image: %s", file);
        return NULL;
    }

    image = SDL_CreateTextureFromSurface(renderer, tmp_image);
    //Free the temporary image
    SDL_FreeSurface(tmp_image);

    return image;
}

/**
 * Draws an image to the renderer
 *
 * @param renderer  renderer to draw on
 * @param image     image to draw
 * @param position  position and size to draw image at (SDL_Rect)
 *                      pass 0 for width and height if you would like
 *                      DrawImage to calculate the width and height
 *                      based on image size.
 * @param clip      clip to draw if available, passing NULL disables clip
 *
 * @return          true if success, otherwise false on failure
 */
bool Painter::DrawImage(SDL_Renderer* renderer, SDL_Texture* image, SDL_Rect* position, SDL_Rect* clip)
{
    if ((clip == NULL) && (position->w == 0) && (position->h == 0)) {
        SDL_QueryTexture(image, NULL, NULL, &position->w, &position->h);
    }

    SDL_RenderCopy(renderer, image, clip, position);

    return true;
}
