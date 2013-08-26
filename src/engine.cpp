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

    last_hover_x = 0;
    last_hover_y = 0;
    move_count = 0;
    current_player = Unit::UNIT_TYPE_X;
    winner = Unit::UNIT_TYPE_NONE;
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
        Initialize Displayer and OpenGL Context
     ****************************************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing window...");
    window = SDL_CreateWindow("SDL2 Tic Tac Toe",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x, y
                WWIDTH, WHEIGHT,    // Window width and height
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing OpenGL Context...");
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    gl_context = SDL_GL_CreateContext(window);

    /***********************************
            Enable Alpha Blending
     ***********************************/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /****************************
        Generate Vertex Array
     ****************************/

    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    /***********************************
            Initialize Program
     **********************************/

    program = Program::Load("shaders/vertexshader.vert", "shaders/fragmentshader.frag", NULL);

    /**********************************
            Initialize Matrices
     **********************************/

    projection_matrix = glm::ortho(0.0f, (float)WWIDTH, (float)WHEIGHT, 0.0f, -1.0f, 1.0f);
    // Translate projection by 0.375 for more precise pixel placements
    projection_matrix *= glm::translate(0.375f, 0.375f, 0.0f);

    view_matrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 1.0f),    // Target vector
            glm::vec3(0.0f, 0.0f, 0.0f),    // Direction vector
            glm::vec3(0.0f, 1.0f, 0.0f));   // Up vector

    /******************************
        Load Rematch Popup
     ******************************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing rematch popup...");
    if (rematch_popup.Initialize(program) == false) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize rematch popup.");
        return false;
    }

    /*****************
        Load Board
     *****************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing board...");
    if (gameboard.Initialize(program) == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game board.");
        return false;
    }

    /*****************
        Load Units
     *****************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
            "Initializing units...");
    if (gameunits.Initialize(program) == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game units.");
        return false;
    }

    /**********************
        Load ScoreBoard
     **********************/

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        "Initializing scoreboard...");
    if (scoreboard.Initialize(program) == false)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialize game scoreboard.");
        return false;
    }

    /*****************************
         Initialize Unit Array
     *****************************/

    ResetBoard();

    /****************
        Finish up
     ****************/

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
        "Game initialized.");
    return true;
}

void Engine::Cleanup()
{
    scoreboard.Cleanup();
    gameboard.Cleanup();
    gameunits.Cleanup();
    rematch_popup.Cleanup();

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array_object);

    SDL_GL_DeleteContext(gl_context);
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
    // Window events
    else if (event->type == SDL_WINDOWEVENT) {
        // Changed size
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            OnWindowResize(event->window.data1, event->window.data2);
        }
    }
}

void Engine::OnLoop()
{

}

void Engine::OnRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gameboard.OnRender(program, projection_matrix, view_matrix);
    gameunits.OnRender(unit_list, program, projection_matrix, view_matrix);
    scoreboard.OnRender(program, projection_matrix, view_matrix);
    
    if (!match_inprogress) {
        rematch_popup.OnRender(program, projection_matrix, view_matrix);
    }

    SDL_GL_SwapWindow(window);
}

/*****************************
        Window Events
 ****************************/
void Engine::OnWindowResize(int width, int height) {
    projection_matrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    // Translate projection by 0.375 for more precise pixel placements
    projection_matrix *= glm::translate(0.375f, 0.375f, 0.0f);
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

/*****************************
         Mouse Events
 ****************************/
void Engine::OnMouseLeftButtonDown(int mouse_x, int mouse_y) {
    // Place player unit only if the match is in progress
    if (match_inprogress) {
        PlaceUnit(mouse_x, mouse_y);
    }
    else
    {
        if (rematch_popup.RematchClicked(mouse_x, mouse_y)) Rematch();
    }
}

void Engine::OnMouseMove(int mouse_x, int mouse_y) {
    // Draw transparent unit of current player at mouse position
    // only if match is in progress
    if (match_inprogress) {
        HoverUnit(mouse_x, mouse_y);
    }
}

/***********************
    Tic Tac Toe Logic
 ***********************/
void Engine::ResetBoard()
{
    move_count = 0;
    current_player = Unit::UNIT_TYPE_X;
    winner = Unit::UNIT_TYPE_NONE;

    // Initialize the units array
    unit_list.clear();
    for (int y = 0; y < 3; y++) {
        std::vector<Unit> tmp_unit_list;

        for (int x = 0; x < 3; x++) {
            Unit tmp_unit;
            tmp_unit_list.push_back(tmp_unit);
        }

        unit_list.push_back(tmp_unit_list);
    }
}

void Engine::Rematch()
{
    ResetBoard();
    match_inprogress = true;
}

void Engine::OnWin()
{
    if (winner == Unit::UNIT_TYPE_X) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Player %i won!", Unit::UNIT_TYPE_X + 1);
        scoreboard.AddPointPlayer1();
        match_inprogress = false;
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Player %i won!", Unit::UNIT_TYPE_O + 1);
        scoreboard.AddPointPlayer2();
        match_inprogress = false;
    }
}

/**
 * Checks through the unit_list(multidimensional array) for a winner
 *
 * @param x The x coordinate of the unit_list
 * @param y The y coordinate of the unit_list
 * @param current_unit_type The unit type that was last assigned. So it knows what to look for.
 *
 */
void Engine::CheckWinner(int x, int y, Unit::Type current_unit_type)
{
    // Check columns
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[x][i].type != current_unit_type)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[x][i].type == Unit::UNIT_TYPE_X) {
                winner = Unit::UNIT_TYPE_X;
                OnWin();
                return;
            } else {
                winner = Unit::UNIT_TYPE_O;
                OnWin();
                return;
            }
        }
    }

    // Check rows
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[i][y].type != current_unit_type)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[i][y].type == Unit::UNIT_TYPE_X) {
                winner = Unit::UNIT_TYPE_X;
                OnWin();
                return;
            } else {
                winner = Unit::UNIT_TYPE_O;
                OnWin();
                return;
            }
        }
    }

    // Check diagonals
    if (x == y) {
        for (unsigned int i = 0; i < unit_list.size(); i++) {
            // If there's any units that aren't placed, then exit loop
            if (unit_list[i][i].type != current_unit_type)
                break;

            // If all units are placed on the board then there must be a clear winner
            if (i == unit_list.size() - 1) {
                // Determine winner (X or O)
                if (unit_list[i][i].type == Unit::UNIT_TYPE_X) {
                    winner = Unit::UNIT_TYPE_X;
                    OnWin();
                    return;
                } else {
                    winner = Unit::UNIT_TYPE_O;
                    OnWin();
                    return;
                }
            }
        }
    }

    // Check Anti Diagonals
    for (unsigned int i = 0; i < unit_list.size(); i++) {
        // If there's any units that aren't placed, then exit loop
        if (unit_list[i][(unit_list.size() - 1) - i].type != current_unit_type)
            break;

        // If all units are placed on the board then there must be a clear winner
        if (i == unit_list.size() - 1) {
            // Determine winner (X or O)
            if (unit_list[i][(unit_list.size() - 1) - i].type == Unit::UNIT_TYPE_X) {
                winner = Unit::UNIT_TYPE_X;
                OnWin();
                return;
            } else {
                winner = Unit::UNIT_TYPE_O;
                OnWin();
                return;
            }
        }
    }

    // Check for Draw
    if (move_count >= 9) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "It's a draw!");
        winner = Unit::UNIT_TYPE_NONE;
        match_inprogress = false;
        return;
    }
}

/**
 * Places/sets a unit into the unit_list, calculated from mouse coordinates
 *
 * @param mouse_x Mouse coordinate x, relative to the window
 * @param mouse_y Mouse coordinate y, relative to the window
 *
 */
void Engine::PlaceUnit(int mouse_x, int mouse_y)
{
    // Check if mouse x and y are actually in the board
    if (mouse_x < TILE_SIZE * ZOOM_LEVEL * 3 && mouse_y < TILE_SIZE * ZOOM_LEVEL * 3) {
        // Divide mouse coordinates by the tile's size to find out where the unit should be put
        int x = mouse_x / (TILE_SIZE * ZOOM_LEVEL);
        int y = mouse_y / (TILE_SIZE * ZOOM_LEVEL);

        Unit::Type current_unit_type = Unit::UNIT_TYPE_NONE;

        // Check if the cell in the unit list hasn't been placed, otherwise it will overwrite existing units
        if (unit_list[x][y].state != Unit::UNIT_STATE_PLACED) {
            move_count++;

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Player %i moves to %ix%i", current_player + 1, x, y);

            if (current_player == Unit::UNIT_TYPE_X) {
                current_unit_type = Unit::UNIT_TYPE_X;
                unit_list[x][y].type = Unit::UNIT_TYPE_X;
                unit_list[x][y].state = Unit::UNIT_STATE_PLACED;
                current_player = Unit::UNIT_TYPE_O;
            }
            else if (current_player == Unit::UNIT_TYPE_O) {
                current_unit_type = Unit::UNIT_TYPE_O;
                unit_list[x][y].type = Unit::UNIT_TYPE_O;
                unit_list[x][y].state = Unit::UNIT_STATE_PLACED;
                current_player = Unit::UNIT_TYPE_X;
            }

            CheckWinner(x, y, current_unit_type);
        }
    }
}

/**
 * When you hover over a tile, it will add a transparent x or o.
 *
 * @param mouse_x Mouse coordinate x, relative to the window.
 * @param mouse_y Mouse coordinate y, relative to the window.
 *
 */
void Engine::HoverUnit(int mouse_x, int mouse_y)
{
    // Check if mouse x and y are actually in the board
    if (mouse_x < TILE_SIZE * ZOOM_LEVEL * 3 && mouse_y < TILE_SIZE * ZOOM_LEVEL * 3) {
        // Divide mouse coordinates by the tile's size to find out where the unit should be put
        int x = mouse_x / (TILE_SIZE * ZOOM_LEVEL);
        int y = mouse_y / (TILE_SIZE * ZOOM_LEVEL);


        // Unset the previously hovered cell (remove transparent unit)
        if (unit_list[last_hover_x][last_hover_y].state == Unit::UNIT_STATE_TRANSPARENT) {
            unit_list[last_hover_x][last_hover_y].state = Unit::UNIT_STATE_NONE;
            unit_list[last_hover_x][last_hover_y].type = Unit::UNIT_TYPE_NONE;
        }

        // Check if the cell in the unit list is NONE, otherwise it will overwrite existing units
        if (unit_list[x][y].state == Unit::UNIT_STATE_NONE) {
            if (current_player == Unit::UNIT_TYPE_X) {
                unit_list[x][y].type = Unit::UNIT_TYPE_X;
                unit_list[x][y].state = Unit::UNIT_STATE_TRANSPARENT;
            }
            else if (current_player == Unit::UNIT_TYPE_O) {
                unit_list[x][y].type = Unit::UNIT_TYPE_O;
                unit_list[x][y].state = Unit::UNIT_STATE_TRANSPARENT;
            }

            // Finally the new last hover is now the cell we set as transparent
            last_hover_x = x;
            last_hover_y = y;
        }
    }
}
