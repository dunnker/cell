#pragma once

#include "member.h"
#include "ga.h"

class XORNetworkMember : public GA::Member
{
public:
    XORNetworkMember();
    ~XORNetworkMember();
    virtual float GetFitness();
    virtual void SetFitness(float value);

    static const unsigned short int INPUT_COUNT = 2;
    static const unsigned short int HIDDEN_COUNT = 2;
    static const unsigned short int OUTPUT_COUNT = 1;

    static const unsigned short int INPUT_NODE_COUNT = INPUT_COUNT + 1; // +1 for bias node
    static const unsigned short int INPUT_WEIGHT_COUNT = HIDDEN_COUNT;

    static const unsigned short int OUTPUT_NODE_COUNT = HIDDEN_COUNT + 1; // +1 for bias node
    static const unsigned short int OUTPUT_WEIGHT_COUNT = OUTPUT_COUNT;

    float** _inputWeights;
    float** _outputWeights;

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
    void EvaluateInputs(XORNetworkMember& member, float inputs[], float expected);
};