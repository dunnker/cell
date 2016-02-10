#pragma once

#include "member.h"
#include "population.h"

namespace GA
{
    template<typename M>
    class Experiment
    {
    public:
        virtual M* NewMember() = 0;
        virtual void EvaluateMember(M& member) = 0;
        virtual void Mutate(M& member) = 0;
        virtual M* NewFromCrossover(M& member1, M& member2) = 0;
    };

    template<typename M>
    class GA
    {
    public:
        /// Create a new population at its first generation
        static Population<M>* NewPopulation(int memberCount, Experiment<M>& experiment);
        /// Evaluate each member of the population and determine their fitness
        static void EvaluatePopulation(Population<M>& population, Experiment<M>& experiment);
        /// Create a new population by choosing random members from an existing population
        static Population<M>* NewGeneration(Population<M>& population, Experiment<M>& experiment);
    private:
        GA(); // static class so make constructor private
    };

    // GA implementation

    template<typename M>
    Population<M>* GA<M>::NewPopulation(int memberCount, Experiment<M>& experiment)
    {
        Population<M>* result = new Population<M>();
        for (int i = 0; i < memberCount; i++)
        {
            result->Add(experiment.NewMember());
        }
        return result;
    }

    template<typename M>
    void GA<M>::EvaluatePopulation(Population<M>& population, Experiment<M>& experiment)
    {
        for (int i = 0; i < population.Count(); i++)
        {
            experiment.EvaluateMember(population.GetMember(i));
        }
    }

    template<typename M>
    Population<M>* GA<M>::NewGeneration(Population<M>& population, Experiment<M>& experiment)
    {
        Population<M>* result = new Population<M>();

        // tournament size is x% of the total population size
        unsigned short int tournamentSize = (unsigned short int)(population.Count() * 0.04);
        if (tournamentSize < 2)
            tournamentSize = 2;

        for (int i = 0; i < population.Count(); i++)
        {
            auto member1 = population.GetRandomMember(tournamentSize);
            auto member2 = population.GetRandomMember(tournamentSize);

            auto newMember = experiment.NewFromCrossover(member1, member2);

            experiment.Mutate(*newMember);

            result->Add(newMember);
        }

        return result;
    }
}