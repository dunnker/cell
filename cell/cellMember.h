#pragma once

#include "member.h"
#include "ga.h"
#include "network.h"

struct Cell
{
    bool alive;
    unsigned short int generation;

    Cell() :
        alive(false),
        generation(0)
    {

    }
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

    NeuralNetwork::Network _network;

    Cell _world[WORLD_SIZE][WORLD_SIZE];

private:
    float _fitness;
};

class CellMemberExperiment : public GA::Experiment<CellMember>
{
public:
    CellMemberExperiment();
    virtual CellMember* NewMember();
    virtual void EvaluateMember(CellMember& member);
    virtual void Mutate(CellMember& member);
    virtual CellMember* NewFromCrossover(CellMember& member1, CellMember& member2);

    bool model[WORLD_SIZE][WORLD_SIZE];

private:
    virtual void EvaluateCell(CellMember& member, int x, int y);
    virtual void EvaluateCellOutputs(CellMember& member, int x, int y);
    virtual void AssignPositionInputs(CellMember& member, unsigned short int index, int x, int y);
};