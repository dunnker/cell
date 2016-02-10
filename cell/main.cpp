#include <iostream>
#include <memory>
#include <time.h>
#include "ga.h"
#include "population.h"
#include "member.h"
#include "xorNetwork.h"

int main()
{
    srand(time(NULL));

    XORNetworkExperiment experiment;

    std::unique_ptr<GA::Population<XORNetworkMember>> population = nullptr;
    for (int generation = 0; generation < 500; generation++)
    {
        if (population == nullptr)
            population = std::make_unique<GA::Population<XORNetworkMember>>(*GA::GA<XORNetworkMember>::NewPopulation(100, experiment));
        else
        {
            auto member = population->GetFittestMember();
            std::cout << generation << " " << member.GetFitness() << "\n";

            if (member.GetFitness() > 1000)
                break;

            population = std::make_unique<GA::Population<XORNetworkMember>>(*GA::GA<XORNetworkMember>::NewGeneration(*population, experiment));
        }

        GA::GA<XORNetworkMember>::EvaluatePopulation(*population, experiment);
    }

    getchar();

    return 0;
}