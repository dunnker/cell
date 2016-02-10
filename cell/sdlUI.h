#pragma once

#include <SDL.h>
#include "cellMember.h"

namespace SDLUI
{
    bool InitEverything();
    bool InitSDL();
    bool CreateWindow();
    bool CreateRenderer();
    void SetupRenderer();

    void Render();
    void RunExperiment(CellMemberExperiment& experiment);
}