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


cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

set (App_Name "sdl-tictactoe")
if (App_Name STREQUAL "")
    message(FATAL_ERROR "You must set an App Name")
endif()

project (${App_Name})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


###
### Find Application Sources
###
file (GLOB SRCS src/*.c src/*.cpp)
file (GLOB HDRS src/*.h src/*.hpp)

link_directories(
)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        )

if(WIN32)
        add_executable(${App_Name} WIN32
        ${SRCS}
        ${HDRS}
        )

        SET_TARGET_PROPERTIES(${App_Name} PROPERTIES COMPILE_FLAGS "")
        SET_TARGET_PROPERTIES(${App_Name} PROPERTIES LINK_FLAGS "-lmingw32 -mwindows")

else ()
        add_executable (${App_Name}
        ${SRCS}
        ${HDRS}
        )
endif ()

target_link_libraries(${App_Name}
)



###
### Find SDL2
###

find_package(SDL2)
if (NOT SDL2_FOUND)
    message (FATAL_ERROR "SDL not found!")
endif (NOT SDL2_FOUND)

find_package(SDL2_image)
if (NOT SDL2IMAGE_FOUND)
    message (FATAL_ERROR "SDL_image not found!")
endif (NOT SDL2IMAGE_FOUND)

###
### Find Boost
###
find_package( Boost COMPONENTS program_options REQUIRED )
if (NOT Boost_FOUND)
    message (FATAL_ERROR "Boost not found!")
endif (NOT Boost_FOUND)

###
### Find OpenGL
###
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message (FATAL_ERROR "OpenGL not found!")
endif(NOT OPENGL_FOUND)

add_definitions(-DGL_GLEXT_PROTOTYPES)

include_directories(
    ${Boost_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${INCLUDE_DIRECTORIES}
    )

target_link_libraries(${App_Name}
    ${Boost_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2MAIN_LIBRARY}
    ${SDL2IMAGE_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${TARGET_LINK_LIBRARIES}
    )

## Copy graphics to output
file(COPY "${CMAKE_SOURCE_DIR}/gfx" DESTINATION "${CMAKE_BINARY_DIR}"
            PATTERN "gfx/*.xcf" EXCLUDE)

file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}")
