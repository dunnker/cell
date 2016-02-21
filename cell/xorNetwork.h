#pragma once

#include "member.h"
#include "ga.h"
#include "network.h"

class XORNetworkMember : public GA::Member
{
public:
    XORNetworkMember();
    ~XORNetworkMember();
    virtual float GetFitness();
    virtual void SetFitness(float value);

    static const unsigned short int INPUT_COUNT = 2;
    static const unsigned short int OUTPUT_COUNT = 1;

    NeuralNetwork::Network _network;

private:
    float _fitness;
};

class XORNetworkExperiment : public GA::Experiment<XORNetworkMember>
{
public:
    virtual XORNetworkMember* NewMember();
    virtual void EvaluateMember(XORNetworkMember& member);
    virtual void Mutate(XORNetworkMember& member);
    virtual XORNetworkMember* NewFromCrossover(XORNetworkMember& member1, XORNetworkMember& member2);
private:
    float GetXORFitness(XORNetworkMember& member, float truthValue, float output);
    void EvaluateInputs(XORNetworkMember& member, float expected);
};