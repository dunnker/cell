#include "sdlUI.h"
#include "consoleUI.h"

#include "xorNetwork.h"
#include "cellMember.h"

int main(int argc, char *args[])
{
    srand((unsigned int)time(NULL));

    //XORNetworkExperiment experiment;
    //ConsoleUI::RunExperiment<XORNetworkMember>(experiment);

    CellMemberExperiment experiment;
    SDLUI::RunExperiment(experiment);

    return 0;
}