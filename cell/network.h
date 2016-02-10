#pragma once

#include <vector>

namespace Network
{
    class Network
    {
    public:
        /// A neural network with a single hidden layer has a set of weight values for inputs, and a set for outputs.
        /// Initialize those input and output weights to random values.
        static void InitializeSingleHiddenLayer(float** inputWeights, unsigned short int inputNodeCount, unsigned short int inputWeightCount, 
            float** outputWeights, unsigned short int outputNodeCount, unsigned short int outputWeightCount);
        /// Calculate the outputs for a neural network with a single hidden layer, given its inputs and weights
        static void FeedForwardSingleHiddenLayer(float inputs[], unsigned short int inputLength, float outputs[], unsigned short int outputLength, 
            float** inputWeights, unsigned short int inputNodeCount, unsigned short int inputWeightCount, 
            float** outputWeights, unsigned short int outputNodeCount, unsigned short int outputWeightCount);
        static float GetRandomFloat();
    private:
        Network(); // static class so make constructor private
    };
}