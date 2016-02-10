#include <assert.h>
#include "cellMember.h"

// CellMember implementation

CellMember::CellMember() :
    _network(INPUT_COUNT, INPUT_COUNT, OUTPUT_COUNT)
{
    assert(false); //init WORLD? fitness?

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

CellMember* CellMemberExperiment::NewMember()
{
    CellMember* result = new CellMember();
    NeuralNetwork::NetworkUtils::InitializeSingleHiddenLayer(result->_network);
    return result;
}

void CellMemberExperiment::EvaluateMember(CellMember& member)
{

}

void CellMemberExperiment::Mutate(CellMember& member)
{
    NeuralNetwork::NetworkUtils::MutateSingleHiddenLayer(member._network);
}

CellMember* CellMemberExperiment::NewFromCrossover(CellMember& member1, CellMember& member2)
{
    auto result = new CellMember();
    NeuralNetwork::NetworkUtils::CrossoverSingleHiddenLayer(member1._network, member2._network, result->_network);
    return result;
}