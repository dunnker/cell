#include "network.h"

namespace Network
{
    class SigmoidActivationFunction
    {
    public:
        static float ProcessValue(float value)
        {
            const float e = 2.71828182f;
            return 1.0f / (1.0f + powf(e, -value));
        }
    };

    float Network::GetRandomFloat()
    {
        static_assert(FLT_MAX > RAND_MAX, "Compile error generating random float");
        return rand() / (float)RAND_MAX;
    }

    void Network::InitializeSingleHiddenLayer(float** inputWeights, unsigned short int inputNodeCount, unsigned short int inputWeightCount,
        float** outputWeights, unsigned short int outputNodeCount, unsigned short int outputWeightCount)
    {
        const float RANGE = 4.0f;

        // assign random weight values between range
        for (int n = 0; n < inputNodeCount; n++)
        {
            for (int w = 0; w < inputWeightCount; w++) 
            {
                inputWeights[n][w] = (GetRandomFloat() * RANGE) - (RANGE / 2.0f);
            }
        }
        for (int n = 0; n < outputNodeCount; n++)
        {
            for (int w = 0; w < outputWeightCount; w++) 
            {
                outputWeights[n][w] = (GetRandomFloat() * RANGE) - (RANGE / 2.0f);
            }
        }
    }

    void Network::FeedForwardSingleHiddenLayer(float inputs[], unsigned short int inputLength, float outputs[], unsigned short int outputLength, 
        float** inputWeights, unsigned short int inputNodeCount, unsigned short int inputWeightCount,
        float** outputWeights, unsigned short int outputNodeCount, unsigned short int outputWeightCount)
    {
        std::vector<float> hidden(inputWeightCount + 1);
        // set bias node
        hidden[hidden.size() - 1] = 1.0f;

        // iterate over nodes of hidden layer, skipping the last bias node
        for (int i = 0; i < (int)hidden.size() - 1; i++) 
        {
            float sum = 0.0f;
            for (int j = 0; j < inputLength; j++)
            {
                sum += inputs[j] * inputWeights[j][i];
            }
            hidden[i] = SigmoidActivationFunction::ProcessValue(sum);
        }

        for (int i = 0; i < outputLength; i++)
        {
            float sum = 0.0f;
            for (int j = 0; j < (int)hidden.size(); j++)
            {
                sum += hidden[j] * outputWeights[j][i];
            }
            outputs[i] = SigmoidActivationFunction::ProcessValue(sum);
        }
    }
}