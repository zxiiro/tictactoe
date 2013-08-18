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

find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES include/SDL2 include
    PATHS ${SDL2_SEARCH_PATHS}
    )

find_library(SDL2_LIBRARY
    NAMES SDL2
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${SDL2_SEARCH_PATHS}
    DOC "SDL2 Lib Path"
    )

find_library(SDL2MAIN_LIBRARY
    NAMES SDL2main
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${SDL2_SEARCH_PATHS}
    )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)

