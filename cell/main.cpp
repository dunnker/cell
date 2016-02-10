#include "sdlUI.h"
#include "consoleUI.h"

#include "xorNetwork.h"
#include "cellMember.h"

int main(int argc, char *args[])
{
    //XORNetworkExperiment experiment;
    //ConsoleUI::RunExperiment<XORNetworkMember>(experiment);

    CellMemberExperiment experiment;
    SDLUI::RunExperiment(experiment);

    return 0;
}