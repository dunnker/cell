#include <iostream>
#include <memory>
#include "sdlUI.h"
#include "population.h"
#include "ga.h"

namespace SDLUI
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    int generation;
    std::unique_ptr<GA::Population<CellMember>> population;

    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;

    const int CELL_SIZE = 20;

    const int MARGIN = 40;

    const int POP_COUNT = 500;

    SDL_Rect CreateCellRect(int x, int y)
    {
        SDL_Rect rect;
        rect.h = CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.x = (x * CELL_SIZE) + MARGIN;
        rect.y = (y * CELL_SIZE) + MARGIN;
        return rect;
    }

    void Render(CellMemberExperiment& experiment)
    {
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect;
        rect.h = (CELL_SIZE * WORLD_SIZE) + 2;
        rect.w = (CELL_SIZE * WORLD_SIZE) + 2;
        rect.x = MARGIN - 1;
        rect.y = MARGIN - 1;
        SDL_RenderDrawRect(renderer, &rect);

        for (int x = 0; x < WORLD_SIZE; x++)
        {
            for (int y = 0; y < WORLD_SIZE; y++)
            {
                if (experiment.model[x][y])
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    SDL_Rect rect = CreateCellRect(x, y);
                    SDL_RenderFillRect(renderer, &rect);
                }

                if (population != nullptr)
                {
                    CellMember& member = population->GetFittestMember();
                    if (member._world[x][y].alive)
                    {
                        if (experiment.model[x][y])
                            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        else
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_Rect rect = CreateCellRect(x, y);
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

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
        SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    void Update()
    {

    }

    void KeyDown(SDL_KeyboardEvent& key, CellMemberExperiment& experiment)
    {
        switch (key.keysym.sym)
        {
            case SDLK_s:
            {
                if (population == nullptr)
                    population = std::make_unique<GA::Population<CellMember>>(*GA::GA<CellMember>::NewPopulation(POP_COUNT, experiment));
                else
                {
                    generation += 1;
                    population = std::make_unique<GA::Population<CellMember>>(*GA::GA<CellMember>::NewGeneration(*population, experiment));
                }

                GA::GA<CellMember>::EvaluatePopulation(*population, experiment);

                CellMember& member = population->GetFittestMember();
                std::cout << "Generation: " << generation << " Best Fitness: " << member.GetFitness() << "\n";

                break;
            }
            default:
                break;
        }
    }

    void MouseLeftButtonDown(SDL_MouseButtonEvent& button, CellMemberExperiment& experiment)
    {
        if (button.x >= MARGIN && button.y >= MARGIN)
        {
            int x = (button.x - MARGIN) / CELL_SIZE;
            int y = (button.y - MARGIN) / CELL_SIZE;
            if (x >= 0 && y >= 0 && x < WORLD_SIZE && y < WORLD_SIZE)
            {
                experiment.model[x][y] = !experiment.model[x][y];
            }
        }
    }

    void RunExperiment(CellMemberExperiment& experiment)
    {
        if (!InitEverything())
            throw "Error initializing SDL";

        bool loop = true;
        while (loop)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    loop = false;
                else if (event.type == SDL_KEYDOWN)
                    KeyDown(event.key, experiment);
                else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                    MouseLeftButtonDown(event.button, experiment);
            }

            Update();

            Render(experiment);

            // Add a 16msec delay to make our game run at ~60 fps
            SDL_Delay(16);
        }
    }
}