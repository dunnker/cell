#pragma once

#include <vector>

namespace NeuralNetwork
{
    struct Network
    {
        static const unsigned short int MAX_HIDDEN_COUNT = 100;
        static const unsigned short int MAX_HIDDEN_LAYERS = 5;

        const unsigned short int _inputCount;
        const unsigned short int _hiddenCount;
        const unsigned short int _outputCount;

        const unsigned short int _hiddenLayerCount;
        const unsigned short int _inputNodeCount;
        const unsigned short int _inputWeightCount;
        const unsigned short int _hiddenNodeCount;
        const unsigned short int _hiddenWeightCount;
        const unsigned short int _outputNodeCount;
        const unsigned short int _outputWeightCount;

        float** _inputWeights; // 2d array [_inputNodeCount][_inputWeightCount]
        float*** _hiddenWeights; // 3d array [_hiddenLayerCount - 1][_hiddenNodeCount][_weightCount]
        float** _outputWeights; // 2d array [_outputNodeCount][_outputWeightCount]

        float* _inputs; // array [_inputNodeCount]; also includes bias node so length is _inputCount + 1
        float* _outputs; // array [_outputCount] length _outputCount NOT _outputNodeCount

        Network(unsigned short int inputCount, unsigned short int hiddenCount, unsigned short int outputCount, unsigned short int hiddenLayerCount = 1) :
            _inputCount(inputCount),
            _hiddenCount(hiddenCount),
            _outputCount(outputCount),
            _inputNodeCount(inputCount + 1),
            _inputWeightCount(hiddenCount),
            _hiddenLayerCount(hiddenLayerCount),
            _hiddenNodeCount(hiddenCount + 1),
            _hiddenWeightCount(hiddenCount),
            _outputNodeCount(hiddenCount + 1),
            _outputWeightCount(outputCount)
        {
            if (hiddenCount > MAX_HIDDEN_COUNT)
                throw "hiddenCount exceeds maximum allowable value";

            _inputs = new float[_inputNodeCount];
            _inputs[_inputNodeCount - 1] = 1.0f; // bias value
            _outputs = new float[_outputCount];

            _inputWeights = new float*[_inputNodeCount];
            for (int i = 0; i < _inputNodeCount; i++)
            {
                _inputWeights[i] = new float[_inputWeightCount];
            }

            if (_hiddenLayerCount > 1)
            {
                _hiddenWeights = new float**[_hiddenLayerCount - 1];
                for (int h = 0; h < _hiddenLayerCount - 1; h++)
                {
                    _hiddenWeights[h] = new float*[_hiddenNodeCount];
                    for (int i = 0; i < _hiddenNodeCount; i++)
                    {
                        _hiddenWeights[h][i] = new float[_hiddenWeightCount];
                    }
                }
            }

            _outputWeights = new float*[_outputNodeCount];
            for (int i = 0; i < _outputNodeCount; i++)
            {
                _outputWeights[i] = new float[_outputWeightCount];
            }
        }

        ~Network()
        {
            delete[] _inputs;
            delete[] _outputs;
            for (int i = 0; i < _inputNodeCount; i++)
                delete[] _inputWeights[i];
            delete[] _inputWeights;
            if (_hiddenLayerCount > 1)
            {
                for (int h = 0; h < _hiddenLayerCount - 1; h++)
                {
                    for (int i = 0; i < _hiddenNodeCount; i++)
                        delete[] _hiddenWeights[h][i];
                    delete[] _hiddenWeights[h];
                }
                delete[] _hiddenWeights;
            }
            for (int i = 0; i < _outputNodeCount; i++)
                delete[] _outputWeights[i];
            delete[] _outputWeights;
        }

        static bool CompareBounds(Network& network1, Network& network2)
        {
            return network1._inputCount == network2._inputCount && network1._hiddenCount == network2._hiddenCount && 
                network1._outputCount == network2._outputCount && network1._hiddenLayerCount == network2._hiddenLayerCount;
        }
    };

    class NetworkUtils
    {
    public:
        /// return random float between 0 and less than 1.0
        static float GetRandomFloat();
        /// A neural network with a single hidden layer has a set of weight values for inputs, and a set for outputs.
        /// Initialize those input and output weights to random values between the given range, e.g. -(range/2) to range/2
        static void Initialize(Network& network, float range = 4.0f);
        /// Calculate the outputs for a neural network with a single hidden layer, given its inputs and weights
        static void FeedForward(Network& network);
        /// Apply a mutation to the weights of a network
        static void Mutate(Network& network, float mutationRate = 0.3f, float mutationRange = 1.0f);
        /// Initialize weight values in network by using a random portion of weights from parent1 and parent2
        static void Crossover(Network& parent1, Network& parent2, Network& network);
    private:
        NetworkUtils(); // static class so make constructor private
    };
}