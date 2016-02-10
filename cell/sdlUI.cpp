#include <iostream>
#include "sdlUI.h"

namespace SDLUI
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;

    void Render()
    {
        // Clear the window and make it all green
        SDL_RenderClear(renderer);

        // Change color to blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        // Render our "player"
        //SDL_RenderFillRect(renderer, &playerPos);

        // Change color to green
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        // Render the changes above
        SDL_RenderPresent(renderer);
    }

    bool InitEverything()
    {
        if (!InitSDL())
            return false;

        if (!CreateWindow())
            return false;

        if (!CreateRenderer())
            return false;

        SetupRenderer();

        return true;
    }

    bool InitSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        {
            std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
            return false;
        }

        return true;
    }

    bool CreateWindow()
    {
        window = SDL_CreateWindow("Server", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        if (window == nullptr)
        {
            std::cout << "Failed to create window : " << SDL_GetError();
            return false;
        }

        return true;
    }

    bool CreateRenderer()
    {
        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer == nullptr)
        {
            std::cout << "Failed to create renderer : " << SDL_GetError();
            return false;
        }

        return true;
    }

    void SetupRenderer()
    {
        // Set size of renderer to the same as window
        SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Set color of renderer to green
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }

    void RunExperiment(CellMemberExperiment& experiment)
    {
        if (!InitEverything())
            return;

        bool loop = true;

        while (loop)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    loop = false;
                else if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        //++playerPos.x;
                        break;
                    case SDLK_LEFT:
                        //--playerPos.x;
                        break;
                        // Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
                    case SDLK_DOWN:
                        //++playerPos.y;
                        break;
                    case SDLK_UP:
                        //--playerPos.y;
                        break;
                    default:
                        break;
                    }
                }
            }

            Render();

            // Add a 16msec delay to make our game run at ~60 fps
            SDL_Delay(16);
        }
    }
}