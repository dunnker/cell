#pragma once

#include "member.h"
#include "ga.h"
#include "network.h"

class ClassifierTestMember : public GA::Member
{
public:
    ClassifierTestMember();
    ~ClassifierTestMember();
    virtual float GetFitness();
    virtual void SetFitness(float value);

    static const unsigned short int INPUT_COUNT = 10;
    static const unsigned short int OUTPUT_COUNT = 5;

    NeuralNetwork::Network _network;

private:
    float _fitness;
};

struct ClassifierTestCase
{
    bool inputs[ClassifierTestMember::INPUT_COUNT];
    int classifier;
};

class ClassifierTestMemberExperiment : public GA::Experiment<ClassifierTestMember>
{
public:
    ClassifierTestMemberExperiment();
    virtual ClassifierTestMember* NewMember();
    virtual void EvaluateMember(ClassifierTestMember& member);
    virtual void Mutate(ClassifierTestMember& member);
    virtual ClassifierTestMember* NewFromCrossover(ClassifierTestMember& member1, ClassifierTestMember& member2);

    ClassifierTestCase _testCases[ClassifierTestMember::OUTPUT_COUNT];

private:
};