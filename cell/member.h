#pragma once

namespace GA
{
    class Member
    {
    public:
        virtual float GetFitness() = 0;
        virtual void SetFitness(float value) = 0;
    };
}