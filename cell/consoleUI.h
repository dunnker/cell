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
        auto population = std::make_unique<GA::Population<M>>(*GA::GA<M>::NewPopulation(400, experiment));
        int generation = 0;
        while (true)
        {
            GA::GA<M>::EvaluatePopulation(*population, experiment);

            M& member = population->GetFittestMember();
            std::cout << "Generation: " << generation << " Best Fitness: " << member.GetFitness() << "        Press <Enter> to continue\n";

            //getchar();

            population = std::make_unique<GA::Population<M>>(*GA::GA<M>::NewGeneration(*population, experiment));
            generation++;
        }
    }
}