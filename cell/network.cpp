#include <assert.h>
#include "network.h"

namespace NeuralNetwork
{
    float NetworkUtils::GetRandomFloat()
    {
        static_assert(FLT_MAX > RAND_MAX + 1, "Compile error generating random float");
        return rand() / (float)(RAND_MAX + 1);
    }

    void NetworkUtils::Initialize(Network& network, float range)
    {
        // assign random weight values between range
        for (int n = 0; n < network._inputNodeCount; n++)
        {
            for (int w = 0; w < network._inputWeightCount; w++) 
            {
                network._inputWeights[n][w] = (GetRandomFloat() * range) - (range / 2.0f);
            }
        }

        if (network._hiddenLayerCount > 1)
        {
            for (int h = 0; h < network._hiddenLayerCount - 1; h++)
            {
                for (int n = 0; n < network._hiddenNodeCount; n++)
                {
                    for (int w = 0; w < network._hiddenWeightCount; w++)
                    {
                        network._hiddenWeights[h][n][w] = (GetRandomFloat() * range) - (range / 2.0f);
                    }
                }
            }
        }

        for (int n = 0; n < network._outputNodeCount; n++)
        {
            for (int w = 0; w < network._outputWeightCount; w++) 
            {
                network._outputWeights[n][w] = (GetRandomFloat() * range) - (range / 2.0f);
            }
        }
    }

    inline float SigmoidActivationFunction(float value)
    {

        //TODO: consider using a faster approximate pow function replacement

        const float e = 2.71828182f;
        return 1.0f / (1.0f + powf(e, -value));
    }

    void NetworkUtils::FeedForward(Network& network)
    {
        unsigned short int hiddenSize = network._inputWeightCount + 1; // +1 for bias
        float hidden[Network::MAX_HIDDEN_LAYERS][Network::MAX_HIDDEN_COUNT]; // fixed size array for performance
        // set bias node
        hidden[0][hiddenSize - 1] = 1.0f;

        // iterate over nodes skipping the last bias node
        for (int i = 0; i < hiddenSize - 1; i++)
        {
            float sum = 0.0f;
            for (int j = 0; j < network._inputNodeCount; j++)
            {
                sum += network._inputs[j] * network._inputWeights[j][i];
            }
            hidden[0][i] = SigmoidActivationFunction(sum);
        }

        if (network._hiddenLayerCount > 1)
        {
            for (int h = 1; h < network._hiddenLayerCount; h++)
            {
                // set bias node
                hidden[h][hiddenSize - 1] = 1.0f;

                // iterate over nodes skipping the last bias node
                for (int i = 0; i < hiddenSize - 1; i++)
                {
                    float sum = 0.0f;
                    for (int j = 0; j < hiddenSize; j++)
                    {
                        sum += hidden[h - 1][j] * network._hiddenWeights[h - 1][j][i];
                    }
                    hidden[h][i] = SigmoidActivationFunction(sum);
                }
            }
        }

        for (int i = 0; i < network._outputCount; i++)
        {
            float sum = 0.0f;

            assert(hiddenSize <= network._outputNodeCount);

            for (int j = 0; j < hiddenSize; j++)
            {
                sum += hidden[network._hiddenLayerCount - 1][j] * network._outputWeights[j][i];
            }
            network._outputs[i] = SigmoidActivationFunction(sum);
        }
    }

    void NetworkUtils::Mutate(Network& network, float mutationRate, float mutationRange)
    {
        for (int i = 0; i < network._inputNodeCount; i++)
        {
            for (int j = 0; j < network._inputWeightCount; j++)
            {
                if (NeuralNetwork::NetworkUtils::GetRandomFloat() < mutationRate)
                    network._inputWeights[i][j] += (mutationRange * NeuralNetwork::NetworkUtils::GetRandomFloat()) - (mutationRange / 2.0f);
            }
        }

        if (network._hiddenLayerCount > 1)
        {
            for (int h = 0; h < network._hiddenLayerCount - 1; h++)
            {
                for (int i = 0; i < network._hiddenNodeCount; i++)
                {
                    for (int j = 0; j < network._hiddenWeightCount; j++)
                    {
                        if (NeuralNetwork::NetworkUtils::GetRandomFloat() < mutationRate)
                            network._hiddenWeights[h][i][j] += (mutationRange * NeuralNetwork::NetworkUtils::GetRandomFloat()) - (mutationRange / 2.0f);
                    }
                }
            }
        }

        for (int i = 0; i < network._outputNodeCount; i++)
        {
            for (int j = 0; j < network._outputWeightCount; j++)
            {
                if (NeuralNetwork::NetworkUtils::GetRandomFloat() < mutationRate)
                    network._outputWeights[i][j] += (mutationRange * NeuralNetwork::NetworkUtils::GetRandomFloat()) - (mutationRange / 2.0f);
            }
        }
    }

    void NetworkUtils::Crossover(Network& parent1, Network& parent2, Network& network)
    {
        assert(NeuralNetwork::Network::CompareBounds(parent1, network));
        assert(NeuralNetwork::Network::CompareBounds(parent2, network));

        // crossover input layer 
        // select random node in parent1, and assign weights from node 0 to that random node, to result
        unsigned short int inputIndex = (unsigned short int)(NeuralNetwork::NetworkUtils::GetRandomFloat() * network._inputNodeCount);
        assert(inputIndex >= 0 && inputIndex < network._inputNodeCount);

        for (int i = 0; i < inputIndex; i++)
        {
            for (int j = 0; j < network._inputWeightCount; j++)
            {
                network._inputWeights[i][j] = parent1._inputWeights[i][j];
            }
        }

        // now assign the rest of the weights from parent2
        for (int i = inputIndex; i < network._inputNodeCount; i++)
        {
            for (int j = 0; j < network._inputWeightCount; j++)
            {
                network._inputWeights[i][j] = parent2._inputWeights[i][j];
            }
        }

        if (network._hiddenLayerCount > 1)
        {
            unsigned short int hiddenIndex = (unsigned short int)(NeuralNetwork::NetworkUtils::GetRandomFloat() * network._hiddenNodeCount);
            assert(hiddenIndex >= 0 && hiddenIndex < network._hiddenNodeCount);

            for (int h = 0; h < network._hiddenLayerCount - 1; h++)
            {
                for (int i = 0; i < hiddenIndex; i++)
                {
                    for (int j = 0; j < network._hiddenWeightCount; j++)
                    {
                        network._hiddenWeights[h][i][j] = parent1._hiddenWeights[h][i][j];
                    }
                }

                for (int i = hiddenIndex; i < network._hiddenNodeCount; i++)
                {
                    for (int j = 0; j < network._hiddenWeightCount; j++)
                    {
                        network._hiddenWeights[h][i][j] = parent2._hiddenWeights[h][i][j];
                    }
                }
            }
        }

        // crossover output layer; similar to steps above for input layer
        unsigned short int outputIndex = (unsigned short int)(NeuralNetwork::NetworkUtils::GetRandomFloat() * network._outputNodeCount);
        assert(outputIndex >= 0 && outputIndex < network._outputNodeCount);

        for (int i = 0; i < outputIndex; i++)
        {
            for (int j = 0; j < network._outputWeightCount; j++)
            {
                network._outputWeights[i][j] = parent1._outputWeights[i][j];
            }
        }

        for (int i = outputIndex; i < network._outputNodeCount; i++)
        {
            for (int j = 0; j < network._outputWeightCount; j++)
            {
                network._outputWeights[i][j] = parent2._outputWeights[i][j];
            }
        }
    }
}