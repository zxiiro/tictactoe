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
 * @param file      filename of image to load
 *
 * @return          returns the loaded image as a GLuint texture
 */
GLuint Painter::LoadImage(const char* file)
{
    SDL_Surface* tmp_image = NULL;
    GLenum texture_format = 0;
    GLint number_of_colors = 0;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Loading image... %s", file);
    tmp_image = IMG_Load(file);
    if (tmp_image == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load image: %s", file);
        return 0;
    }

    number_of_colors = tmp_image->format->BytesPerPixel;
    
    // Determine texture format
    if (number_of_colors == 4) {
        if (tmp_image->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else if (number_of_colors == 3) {
        if (tmp_image->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Failed to determine texture format from: %s", file);
    }

    // Create texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture's stretching and shrinking properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load image into data that opengl can read and process
    glTexImage2D(GL_TEXTURE_2D,     // Target
            0,                      // Levels
            number_of_colors,       // Internal Format
            tmp_image->w,           // Width
            tmp_image->h,           // Height
            0,                      // Border
            texture_format,         // Texture Format
            GL_UNSIGNED_BYTE,       // Type
            tmp_image->pixels);     // Image Data

    //Free the temporary image
    SDL_FreeSurface(tmp_image);

    if (textureID == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Failed to convert SDL_Surface to GLuint from: %s", file);
        return 0;
    }

    return textureID;
}
