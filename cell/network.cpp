#include <assert.h>
#include "network.h"

namespace NeuralNetwork
{
    float NetworkUtils::GetRandomFloat()
    {
        static_assert(FLT_MAX > RAND_MAX + 1, "Compile error generating random float");
        return rand() / (float)(RAND_MAX + 1);
    }

    void NetworkUtils::InitializeSingleHiddenLayer(SingleHiddenLayerNetwork& network, float range)
    {
        // assign random weight values between range
        for (int n = 0; n < network._inputNodeCount; n++)
        {
            for (int w = 0; w < network._inputWeightCount; w++) 
            {
                network._inputWeights[n][w] = (GetRandomFloat() * range) - (range / 2.0f);
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

    void NetworkUtils::FeedForwardSingleHiddenLayer(SingleHiddenLayerNetwork& network)
    {
        unsigned short int hiddenSize = network._inputWeightCount + 1;
        float hidden[SingleHiddenLayerNetwork::MAX_HIDDEN_COUNT]; // static sized array for performance
        // set bias node
        hidden[hiddenSize - 1] = 1.0f;

        // iterate over nodes of hidden layer, skipping the last bias node
        for (int i = 0; i < hiddenSize - 1; i++)
        {
            float sum = 0.0f;
            for (int j = 0; j < network._inputNodeCount; j++)
            {
                sum += network._inputs[j] * network._inputWeights[j][i];
            }
            hidden[i] = SigmoidActivationFunction(sum);
        }

        for (int i = 0; i < network._outputCount; i++)
        {
            float sum = 0.0f;

            assert(hiddenSize <= network._outputNodeCount);

            for (int j = 0; j < hiddenSize; j++)
            {
                sum += hidden[j] * network._outputWeights[j][i];
            }
            network._outputs[i] = SigmoidActivationFunction(sum);
        }
    }

    void NetworkUtils::MutateSingleHiddenLayer(SingleHiddenLayerNetwork& network, float mutationRate, float mutationRange)
    {
        for (int i = 0; i < network._inputNodeCount; i++)
        {
            for (int j = 0; j < network._inputWeightCount; j++)
            {
                if (NeuralNetwork::NetworkUtils::GetRandomFloat() < mutationRate)
                    network._inputWeights[i][j] += (mutationRange * NeuralNetwork::NetworkUtils::GetRandomFloat()) - (mutationRange / 2.0f);
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

    void NetworkUtils::CrossoverSingleHiddenLayer(SingleHiddenLayerNetwork& parent1, SingleHiddenLayerNetwork& parent2, SingleHiddenLayerNetwork& network)
    {
        assert(NeuralNetwork::SingleHiddenLayerNetwork::CompareBounds(parent1, network));
        assert(NeuralNetwork::SingleHiddenLayerNetwork::CompareBounds(parent2, network));

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