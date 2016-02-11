#include <assert.h>
#include "network.h"
#include "cellMember.h"

// CellMember implementation

CellMember::CellMember() :
    _fitness(0),
    _network(INPUT_COUNT, INPUT_COUNT, OUTPUT_COUNT)
{
    _world[0][0].alive = true;
}

CellMember::~CellMember()
{

}

float CellMember::GetFitness()
{
    return _fitness;
}

void CellMember::SetFitness(float value)
{
    _fitness = value;
}

// CellMemberExperiment implementation

CellMemberExperiment::CellMemberExperiment()
{
    std::memset(model, 0, sizeof model);
}

CellMember* CellMemberExperiment::NewMember()
{
    CellMember* result = new CellMember();
    NeuralNetwork::NetworkUtils::InitializeSingleHiddenLayer(result->_network);
    return result;
}

void CellMemberExperiment::AssignPositionInputs(CellMember& member, unsigned short int index, int x, int y)
{
    if (x >= 0 && x < WORLD_SIZE && y >= 0 && y < WORLD_SIZE && member._world[x][y].alive)
        member._network._inputs[index] = 1.0f;
    else 
        member._network._inputs[index] = 0.0f;
}

void CellMemberExperiment::EvaluateCellOutputs(CellMember& member, int x, int y)
{
    if (member._network._outputs[0] > 0.5f || member._network._outputs[0] < -0.5f)
    {
        int nextX = 0;
        int nextY = 0;
        if (member._network._outputs[1] > 0.5f)
            nextX = 1;
        else if (member._network._outputs[1] < -0.5f)
            nextX = -1;
        if (member._network._outputs[2] > 0.5f)
            nextY = 1;
        else if (member._network._outputs[2] < -0.5f)
            nextY = -1;
        if (nextX != 0 || nextY != 0)
        {
            nextX += x;
            nextY += y;
            if (nextX >= 0 && nextX < WORLD_SIZE && nextY >= 0 && nextY < WORLD_SIZE && !member._world[nextX][nextY].alive)
            {
                member._world[nextX][nextY].alive = true;
                member._world[nextX][nextY].generation = member._world[x][y].generation + 1;
            }
        }
    }

    if (member._network._outputs[3] < -0.5)
        member._world[x][y].alive = false;
    else if (member._network._outputs[3] > 0.5)
        member._world[x][y].alive = false;
}

void CellMemberExperiment::EvaluateCell(CellMember& member, int x, int y)
{
    AssignPositionInputs(member, 0, x - 1, y - 1);
    AssignPositionInputs(member, 1, x, y - 1);
    AssignPositionInputs(member, 2, x + 1, y - 1);

    AssignPositionInputs(member, 3, x - 1, y);
    AssignPositionInputs(member, 4, x + 1, y);

    AssignPositionInputs(member, 5, x - 1, y + 1);
    AssignPositionInputs(member, 6, x, y + 1);
    AssignPositionInputs(member, 7, x + 1, y + 1);

    member._network._inputs[8] = member._world[x][y].generation;
    assert(member._network._inputs[9] > 0.999f && member._network._inputs[9] < 1.001f);

    NeuralNetwork::NetworkUtils::FeedForwardSingleHiddenLayer(member._network);
}

void CellMemberExperiment::EvaluateMember(CellMember& member)
{
    const int TURN_COUNT = WORLD_SIZE * WORLD_SIZE;

    // evaluate cells in member's world

    for (int i = 0; i < TURN_COUNT; i++)
    {
        for (int x = 0; x < WORLD_SIZE; x++)
        {
            for (int y = 0; y < WORLD_SIZE; y++)
            {
                if (member._world[x][y].alive)
                {
                    EvaluateCell(member, x, y);
                    EvaluateCellOutputs(member, x, y);
                }
            }
        }
    }

    // determine fitness

    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            if (model[x][y])
            {
                if (member._world[x][y].alive)
                    member.SetFitness(member.GetFitness() + 10.0f);
                else 
                    member.SetFitness(member.GetFitness() - 1.0f);
            }
            else 
            {
                if (member._world[x][y].alive)
                    member.SetFitness(member.GetFitness() - 5.0f);
                else
                    member.SetFitness(member.GetFitness() + 0.5f);
            }
        }
    }
}

void CellMemberExperiment::Mutate(CellMember& member)
{
    NeuralNetwork::NetworkUtils::MutateSingleHiddenLayer(member._network);
}

CellMember* CellMemberExperiment::NewFromCrossover(CellMember& member1, CellMember& member2)
{
    CellMember* result = new CellMember();
    NeuralNetwork::NetworkUtils::CrossoverSingleHiddenLayer(member1._network, member2._network, result->_network);
    return result;
}