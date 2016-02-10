#pragma once

#include "member.h"
#include "ga.h"

struct Cell
{
    bool alive;
    unsigned short int generation;
};

const unsigned int WORLD_SIZE = 10;

class CellMember : public GA::Member
{
public:
    CellMember();
    ~CellMember();
    virtual float GetFitness();
    virtual void SetFitness(float value);

    static const unsigned short int INPUT_COUNT = 9;
    static const unsigned short int OUTPUT_COUNT = 4;

    Cell _world[WORLD_SIZE - 1][WORLD_SIZE - 1];

private:
    float _fitness;
};

class CellMemberExperiment : public GA::Experiment<CellMember>
{
public:
    virtual CellMember* NewMember();
    virtual void EvaluateMember(CellMember& member);
    virtual void Mutate(CellMember& member);
    virtual CellMember* NewFromCrossover(CellMember& member1, CellMember& member2);
private:
};