#pragma once

#include <type_traits>
#include <vector>
#include "member.h"

namespace GA
{
    template<typename M>
    class Population
    {
    public:
        Population();
        ~Population();
        void Add(M* member);
        M& GetMember(int index);
        /// Use a tournament selection strategy to select a random member from the population
        M& GetRandomMember(unsigned short int tournamentSize);
        M& GetFittestMember();
        int Count();
    private:
        std::vector<M*> _members;
    };

    // Population implementation

    template<typename M>
    Population<M>::Population() :
        _members()
    {
        static_assert(std::is_base_of<Member, M>::value, "Type parameter of this class must derive from Member");
    };

    template<typename M>
    Population<M>::~Population()
    {
        for (auto member : _members)
        {
            delete(member);
        }
    }

    template<typename M>
    void Population<M>::Add(M* member)
    {
        _members.push_back(member);
    }

    template<typename M>
    M& Population<M>::GetMember(int index)
    {
        return *_members[index];
    }

    template<typename M>
    M& Population<M>::GetRandomMember(unsigned short int tournamentSize)
    {
        if (Count() < 2)
            throw "Population size is too small";
        if (tournamentSize < 2)
            throw "Invalid tournament size";

        // select random members up to the tournament size
        // out of those members, find which has the greatest fitness
        M* result = nullptr;
        for (int i = 0; i < tournamentSize; i++)
        {
            int index = rand() % Count();
            if (result == nullptr || _members[index]->GetFitness() > result->GetFitness())
                result = _members[index];
        }

        return *result;
    }

    template<typename M>
    M& Population<M>::GetFittestMember()
    {
        M* result = nullptr;
        for (auto member : _members)
        {
            if (result == nullptr || member->GetFitness() > result->GetFitness())
                result = member;
        }
        return *result;
    }

    template<typename M>
    int Population<M>::Count()
    {
        return _members.size();
    }
}