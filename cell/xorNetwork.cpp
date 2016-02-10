#include "xorNetwork.h"
#include "network.h"

// XORNetworkMember implementation

XORNetworkMember::XORNetworkMember() :
    _fitness(0)
{
    _inputWeights = new float*[INPUT_NODE_COUNT];
    for (int i = 0; i < INPUT_NODE_COUNT; i++)
    {
        _inputWeights[i] = new float[INPUT_WEIGHT_COUNT];
    }
    _outputWeights = new float*[OUTPUT_NODE_COUNT];
    for (int i = 0; i < OUTPUT_NODE_COUNT; i++)
    {
        _outputWeights[i] = new float[OUTPUT_WEIGHT_COUNT];
    }
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
    Network::Network::InitializeSingleHiddenLayer(result->_inputWeights, XORNetworkMember::INPUT_NODE_COUNT, XORNetworkMember::INPUT_WEIGHT_COUNT, 
        result->_outputWeights, XORNetworkMember::OUTPUT_NODE_COUNT, XORNetworkMember::OUTPUT_WEIGHT_COUNT);
    return result;
}

float XORNetworkExperiment::GetXORFitness(XORNetworkMember& member, float truthValue, float output)
{
    return member.GetFitness() + powf(abs(truthValue - output), 2.0f);
}

void XORNetworkExperiment::EvaluateInputs(XORNetworkMember& member, float inputs[], float expected)
{
    float outputs[XORNetworkMember::OUTPUT_COUNT];
    Network::Network::FeedForwardSingleHiddenLayer(inputs, XORNetworkMember::INPUT_NODE_COUNT,
        outputs, XORNetworkMember::OUTPUT_COUNT, member._inputWeights, XORNetworkMember::INPUT_NODE_COUNT, XORNetworkMember::INPUT_WEIGHT_COUNT, 
        member._outputWeights, XORNetworkMember::OUTPUT_NODE_COUNT, XORNetworkMember::OUTPUT_WEIGHT_COUNT);
    member.SetFitness(GetXORFitness(member, expected, outputs[0]));
}

void XORNetworkExperiment::EvaluateMember(XORNetworkMember& member)
{
    // test T xor T, so inputs are 1.0 and 1.0, but also bias node is 1.0
    float inputs[XORNetworkMember::INPUT_NODE_COUNT] = { 1.0f, 1.0f, 1.0f };
    EvaluateInputs(member, inputs, 0.0f);

    // test T xor F
    float inputs2[XORNetworkMember::INPUT_NODE_COUNT] = { 1.0f, 0.0f, 1.0f };
    EvaluateInputs(member, inputs2, 1.0f);

    // test F xor T
    float inputs3[XORNetworkMember::INPUT_NODE_COUNT] = { 0.0f, 1.0f, 1.0f };
    EvaluateInputs(member, inputs3, 1.0f);

    // test F xor F
    float inputs4[XORNetworkMember::INPUT_NODE_COUNT] = { 0.0f, 0.0f, 1.0f };
    EvaluateInputs(member, inputs4, 0.0f);

    if (member.GetFitness() > 0.0f)
        member.SetFitness(1.0f / member.GetFitness()); // amplify fitness, helps successful member to stand out among peers
    else
        member.SetFitness(1.0f);
}

void XORNetworkExperiment::Mutate(XORNetworkMember& member)
{
    const float MUTATION_RATE = 0.3f;
    const float MUTATION_RANGE = 0.5f;

    for (int i = 0; i < XORNetworkMember::INPUT_NODE_COUNT; i++)
    {
        for (int j = 0; j < XORNetworkMember::INPUT_WEIGHT_COUNT; j++)
        {
            if (Network::Network::GetRandomFloat() < MUTATION_RATE)
                member._inputWeights[i][j] += (MUTATION_RANGE * Network::Network::GetRandomFloat()) - (MUTATION_RANGE / 2.0f);
        }
    }

    for (int i = 0; i < XORNetworkMember::OUTPUT_NODE_COUNT; i++)
    {
        for (int j = 0; j < XORNetworkMember::OUTPUT_WEIGHT_COUNT; j++)
        {
            if (Network::Network::GetRandomFloat() < MUTATION_RATE)
                member._outputWeights[i][j] += (MUTATION_RANGE * Network::Network::GetRandomFloat()) - (MUTATION_RANGE / 2.0f);
        }
    }
}

XORNetworkMember* XORNetworkExperiment::NewFromCrossover(XORNetworkMember& member1, XORNetworkMember& member2)
{
    auto result = new XORNetworkMember();

    // crossover input layer 
    // select random node in member1, and assign weights from node 0 to that random node, to result
    unsigned short int inputIndex = (unsigned short int)(Network::Network::GetRandomFloat() * XORNetworkMember::INPUT_NODE_COUNT);
    for (int i = 0; i < inputIndex; i++)
    {
        for (int j = 0; j < XORNetworkMember::INPUT_WEIGHT_COUNT; j++)
        {
            result->_inputWeights[i][j] = member1._inputWeights[i][j];
        }
    }

    // now assign the rest of the weights from member2
    for (int i = inputIndex; i < XORNetworkMember::INPUT_NODE_COUNT; i++)
    {
        for (int j = 0; j < XORNetworkMember::INPUT_WEIGHT_COUNT; j++)
        {
            result->_inputWeights[i][j] = member2._inputWeights[i][j];
        }
    }

    // crossover output layer; similar to steps above for input layer
    unsigned short int outputIndex = (unsigned short int)(Network::Network::GetRandomFloat() * XORNetworkMember::OUTPUT_NODE_COUNT);
    for (int i = 0; i < outputIndex; i++)
    {
        for (int j = 0; j < XORNetworkMember::OUTPUT_WEIGHT_COUNT; j++)
        {
            result->_outputWeights[i][j] = member1._outputWeights[i][j];
        }
    }

    for (int i = outputIndex; i < XORNetworkMember::OUTPUT_NODE_COUNT; i++)
    {
        for (int j = 0; j < XORNetworkMember::OUTPUT_WEIGHT_COUNT; j++)
        {
            result->_outputWeights[i][j] = member2._outputWeights[i][j];
        }
    }

    return result;
}