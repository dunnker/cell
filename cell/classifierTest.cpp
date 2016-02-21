#include <assert.h>
#include "network.h"
#include "classifierTest.h"

// ClassifierTestMember implementation

ClassifierTestMember::ClassifierTestMember() :
    _fitness(0),
    _network(INPUT_COUNT, INPUT_COUNT, OUTPUT_COUNT, 1)
{
    
}

ClassifierTestMember::~ClassifierTestMember()
{

}

float ClassifierTestMember::GetFitness()
{
    return _fitness;
}

void ClassifierTestMember::SetFitness(float value)
{
    _fitness = value;
}

// ClassifierTestMemberExperiment implementation

ClassifierTestMemberExperiment::ClassifierTestMemberExperiment()
{
    for (int i = 0; i < ClassifierTestMember::OUTPUT_COUNT; i++)
    {

        // this test case is expected to have an output at node number i
        // all other nodes should have a minimal value...
        _testCases[i].classifier = i;

        for (int j = 0; j < ClassifierTestMember::INPUT_COUNT; j++)
        {
            // init the test case so that some percentage of its inputs will emit a value
            _testCases[i].inputs[j] = NeuralNetwork::NetworkUtils::GetRandomFloat() < 0.3f;
        }
    }
}

ClassifierTestMember* ClassifierTestMemberExperiment::NewMember()
{
    ClassifierTestMember* result = new ClassifierTestMember();
    NeuralNetwork::NetworkUtils::Initialize(result->_network);
    return result;
}

void ClassifierTestMemberExperiment::EvaluateMember(ClassifierTestMember& member)
{
    float fitness = 0.0f;
    for (int i = 0; i < ClassifierTestMember::OUTPUT_COUNT; i++)
    {
        for (int j = 0; j < ClassifierTestMember::INPUT_COUNT; j++)
        {
            if (_testCases[i].inputs[j])
                member._network._inputs[j] = 1.0f;
            else
                member._network._inputs[j] = 0.0f;
        }
        NeuralNetwork::NetworkUtils::FeedForward(member._network);
        for (int k = 0; k < ClassifierTestMember::OUTPUT_COUNT; k++)
        {
            if (member._network._outputs[k] > 0.8f)
            {
                if (_testCases[i].classifier == k)
                    fitness += 1.0f;
                else
                    // false spike
                    fitness -= 1.0f;
            }
            else
            {
                if (_testCases[i].classifier == k)
                    // missed spike
                    fitness -= 1.0f;
                else if (member._network._outputs[k] < 0.2f)
                    // correct null output
                    fitness += 1.0f;
            }
        }
    }
    // max fitness would be OUTPUT_COUNT ^ 2
    member.SetFitness(fitness);
}

void ClassifierTestMemberExperiment::Mutate(ClassifierTestMember& member)
{
    NeuralNetwork::NetworkUtils::Mutate(member._network, 0.3f, 2.0f);
}

ClassifierTestMember* ClassifierTestMemberExperiment::NewFromCrossover(ClassifierTestMember& member1, ClassifierTestMember& member2)
{
    ClassifierTestMember* result = new ClassifierTestMember();
    NeuralNetwork::NetworkUtils::Crossover(member1._network, member2._network, result->_network);
    return result;
}