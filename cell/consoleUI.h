#pragma once

#include <time.h>
#include <iostream>
#include <memory>
#include "population.h"
#include "ga.h"

namespace ConsoleUI
{
    template<typename M>
    void RunExperiment(GA::Experiment<M>& experiment)
    {
        srand((unsigned int)time(NULL));

        std::unique_ptr<GA::Population<M>> population = nullptr;
        for (int generation = 0; generation < 500; generation++)
        {
            if (population == nullptr)
                population = std::make_unique<GA::Population<M>>(*GA::GA<M>::NewPopulation(100, experiment));
            else
            {
                auto member = population->GetFittestMember();
                std::cout << generation << " " << member.GetFitness() << "\n";

                if (member.GetFitness() > 1000)
                    break;

                population = std::make_unique<GA::Population<M>>(*GA::GA<M>::NewGeneration(*population, experiment));
            }

            GA::GA<M>::EvaluatePopulation(*population, experiment);
        }

        getchar();
    }
}