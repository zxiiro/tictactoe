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

#include "engine.h"

Engine::Engine()
{
    running = true;
    match_inprogress = true;
};

/**************************************
    Initialization and Cleanup
 **************************************/

bool Engine::Initialize()
{
    // Initialize SDL or fail if initialization issue
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing SDL...");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }


    /****************************************
        Initialize Displayer and Renderer
     ****************************************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing window...");
    window = SDL_CreateWindow("SDL2 Tic Tac Toe",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x, y
                WWIDTH, WHEIGHT,    // Window width and height
                SDL_WINDOW_RESIZABLE);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing renderer...");
    renderer = SDL_CreateRenderer(window, -1,
                    SDL_RENDERER_TARGETTEXTURE |
                    SDL_RENDERER_PRESENTVSYNC |
                    SDL_RENDERER_ACCELERATED);


    /*****************
        Load Board
     *****************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing board...");
    if (Board::GameBoard.Initialize() == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game board.");
    }
    Board::GameBoard.tileset = Painter::LoadImage(renderer, "gfx/tiles.png");

    /*****************
        Load Units
     *****************/
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Initializing units...");
    if (Unit::GameUnits.Initialize() == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game units.");
    }
    Unit::GameUnits.unitset = Painter::LoadImage(renderer, "gfx/units.png");

    /**********************
        Load ScoreBoard
     **********************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing scoreboard...");
    if (ScoreBoard::GameScoreBoard.Initialize(renderer) == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game scoreboard.");
    }
    ScoreBoard::GameScoreBoard.scoreboardset = Painter::LoadImage(renderer, "gfx/scoreboard.png");

    /****************
        Finish up
     ****************/

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
        "Game initialized.");
    return true;
}

void Engine::Cleanup()
{

    // Cleanup items created by Initialize()
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Engine::Execute()
{
    if (Initialize() == false) {
        return -1;
    }

    SDL_Event event;

    // Game loop
    while (running) {
        // Poll for SDL Events
        while (SDL_PollEvent(&event)) {
            OnEvent(&event);
        }

        OnLoop();
        OnRender();
    }

    // Cleanup and return
    Cleanup();
    return 0;
}

/*****************************
    Events
 *****************************/

void Engine::OnEvent(SDL_Event* event)
{
    // User clicked the close button
    if(event->type == SDL_QUIT) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "Exiting Tic Tac Toe");
        running = false;
    }
    // User is left clicking
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            OnMouseLeftButtonDown(event->button.x, event->button.y);
        }
    }
    // User's mouse is moving
    else if (event->type == SDL_MOUSEMOTION) {
        OnMouseMove(event->motion.x, event->motion.y);
    }
}

void Engine::OnLoop()
{

}

void Engine::OnRender()
{
    SDL_RenderClear(renderer);

    Board::GameBoard.OnRender(renderer);
    Unit::GameUnits.OnRender(renderer);
    ScoreBoard::GameScoreBoard.OnRender(renderer);

    SDL_RenderPresent(renderer);
}

/*****************************
         Mouse Events
 ****************************/
void Engine::OnMouseLeftButtonDown(int mouse_x, int mouse_y) {
    // Check if mouse x and y are actually in the board
    if (mouse_x < TILE_SIZE * ZOOM_LEVEL * 3 && mouse_y < TILE_SIZE * ZOOM_LEVEL * 3) {
        // Divide mouse coordinates by the tile's size to find out where the unit should be put
        int x = mouse_x / (TILE_SIZE * ZOOM_LEVEL);
        int y = mouse_y / (TILE_SIZE * ZOOM_LEVEL);

        // Place player unit only if the match is in progress
        if (match_inprogress) Unit::GameUnits.SetCell(x, y);
    }
}

void Engine::OnMouseMove(int mouse_x, int mouse_y) {
    // Check if mouse x and y are actually in the board
    if (mouse_x < TILE_SIZE * ZOOM_LEVEL * 3 && mouse_y < TILE_SIZE * ZOOM_LEVEL * 3) {
        // Divide mouse coordinates by the tile's size to find out where the unit should be put
        int x = mouse_x / (TILE_SIZE * ZOOM_LEVEL);
        int y = mouse_y / (TILE_SIZE * ZOOM_LEVEL);

        // Draw transparent unit of current player at mouse position
        // only if match is in progress
        if (match_inprogress) Unit::GameUnits.SetTransparentCell(x, y);
    }
}
