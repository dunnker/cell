#include "sdlUI.h"
#include "consoleUI.h"

#include "xorNetwork.h"
#include "cellMember.h"
#include "classifierTest.h"

int main(int argc, char *args[])
{
    srand((unsigned int)time(NULL));

    //XORNetworkExperiment experiment;
    //ConsoleUI::RunExperiment<XORNetworkMember>(experiment);

    ClassifierTestMemberExperiment experiment;
    ConsoleUI::RunExperiment<ClassifierTestMember>(experiment);

    //CellMemberExperiment experiment;
    //SDLUI::RunExperiment(experiment);

    return 0;
}