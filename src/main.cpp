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

#ifdef ANDROID
    #include <SDL.h>
#else
    #include <boost/program_options.hpp>
    namespace po = boost::program_options;
    #include <SDL2/SDL.h>
#endif // ANDROID

#include "engine.h"

int main(int argc, char** argv)
{
    /********************************
        Parse commandline options
     ********************************/
    #ifndef ANDROID
    po::options_description desc("Allowed options");
    desc.add_options()
        ("debug", "enable debug logs")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Enable Debug Logs
    if (vm.count("debug")) {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    }
    #endif // ANDROID

    /****************************
        Start the game Engine
     ****************************/
    Engine game;
    return game.Execute();
}
