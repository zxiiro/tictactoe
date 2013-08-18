#  Copyright (c) 2013 Thanh Ha
#
#  This file is part of sdl-tictactoe.
#
#  sdl-tictactoe is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  sdl-tictactoe is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with sdl-tictactoe.  If not, see <http://www.gnu.org/licenses/>.

set(SDL2_SEARCH_PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )

find_path(SDL2_INCLUDE_DIR SDL_image.h
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES include/SDL2 include
    PATHS ${SDL2_SEARCH_PATHS}
    )

find_library(SDL2IMAGE_LIBRARY
    NAMES SDL2_image
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${SDL2_SEARCH_PATHS}
    DOC "SDL2 image Library Path"
    )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2_image REQUIRED_VARS SDL2IMAGE_LIBRARY SDL2_INCLUDE_DIR) 

set(SDL2IMAGE_FOUND "NO")
if(SDL2IMAGE_LIBRARY AND SDL2_INCLUDE_DIR)
set(SDL2IMAGE_FOUND "YES")
endif(SDL2IMAGE_LIBRARY AND SDL2_INCLUDE_DIR)

