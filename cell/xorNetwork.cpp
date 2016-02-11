#include "xorNetwork.h"
#include "network.h"

// XORNetworkMember implementation

XORNetworkMember::XORNetworkMember() :
    _fitness(0),
    _network(INPUT_COUNT, INPUT_COUNT, OUTPUT_COUNT)
{

}

XORNetworkMember::~XORNetworkMember()
{

}

float XORNetworkMember::GetFitness()
{
    return _fitness;
}

void XORNetworkMember::SetFitness(float value)
{
    _fitness = value;
}

// XORNetworkExperiment implementation

XORNetworkMember* XORNetworkExperiment::NewMember()
{
    XORNetworkMember* result = new XORNetworkMember();
    NeuralNetwork::NetworkUtils::InitializeSingleHiddenLayer(result->_network);
    return result;
}

float XORNetworkExperiment::GetXORFitness(XORNetworkMember& member, float truthValue, float output)
{
    return member.GetFitness() + powf(abs(truthValue - output), 2.0f);
}

void XORNetworkExperiment::EvaluateInputs(XORNetworkMember& member, float expected)
{
    NeuralNetwork::NetworkUtils::FeedForwardSingleHiddenLayer(member._network);
    member.SetFitness(GetXORFitness(member, expected, member._network._outputs[0]));
}

void XORNetworkExperiment::EvaluateMember(XORNetworkMember& member)
{
    // test T xor T, so inputs are 1.0 and 1.0 (note there is an additional bias node of 1.0)
    member._network._inputs[0] = 1.0f;
    member._network._inputs[1] = 1.0f;
    EvaluateInputs(member, 0.0f);

    // test T xor F
    member._network._inputs[0] = 1.0f;
    member._network._inputs[1] = 0.0f;
    EvaluateInputs(member, 1.0f);

    // test F xor T
    member._network._inputs[0] = 0.0f;
    member._network._inputs[1] = 1.0f;
    EvaluateInputs(member, 1.0f);

    // test F xor F
    member._network._inputs[0] = 0.0f;
    member._network._inputs[1] = 0.0f;
    EvaluateInputs(member, 0.0f);

    if (member.GetFitness() > 0.0f)
        member.SetFitness(1.0f / member.GetFitness()); // amplify fitness, helps successful member to stand out among peers
    else
        member.SetFitness(1.0f);
}

void XORNetworkExperiment::Mutate(XORNetworkMember& member)
{
    NeuralNetwork::NetworkUtils::MutateSingleHiddenLayer(member._network);
}

XORNetworkMember* XORNetworkExperiment::NewFromCrossover(XORNetworkMember& member1, XORNetworkMember& member2)
{
    XORNetworkMember* result = new XORNetworkMember();
    NeuralNetwork::NetworkUtils::CrossoverSingleHiddenLayer(member1._network, member2._network, result->_network);
    return result;
}