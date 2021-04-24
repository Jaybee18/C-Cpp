#include "Layer.h"
#include <iostream>

using namespace std;

class NeuralNetwork
{
private:
    vector<Layer> layers;

public:
    NeuralNetwork(vector<int> noNeurons, int noInputs);
    vector<double> forwardPass(vector<double> input);
    void train(vector<vector<double>> input, vector<vector<double>> output, int iterations);
    void show();
    ~NeuralNetwork();
};

NeuralNetwork::NeuralNetwork(vector<int> noNeurons, int noInputs)
{
    int lastOutputs = noInputs;
    for (int i = 0; i < noNeurons.size(); i++)
    {
        Layer temp = Layer(noNeurons[i], lastOutputs);
        layers.push_back(temp);
        lastOutputs = noNeurons[i];
    }
}

vector<double> NeuralNetwork::forwardPass(vector<double> input)
{
    vector<double> outputs_lastLayer = input;
    for (int i = 0; i < layers.size(); i++)
        outputs_lastLayer = layers[i].forwardPass(outputs_lastLayer);
    return outputs_lastLayer;
}

void NeuralNetwork::train(vector<vector<double>> input, vector<vector<double>> output, int iterations)
{
    for (int iteration = 0; iteration < iterations; iteration++)
    {
        for (int dataset = 0; dataset < input.size(); dataset++)
        {
            // forward pass
            vector<vector<double>> outputs;
            vector<double> lastOutput = input[dataset];
            for (int i = 0; i < layers.size(); i++)
            {
                vector<double> tempOutput = layers[i].forwardPass(input[dataset]);
                outputs.push_back(tempOutput);
                lastOutput = tempOutput;
            }

            // backpropagation
            /* calculate the mse wrt each of the activations */
            vector<double> error = layers[layers.size() - 1].calcError(output[dataset]);

            layers[layers.size() - 1].calcWeightDeltas(error);
            vector<double> grad = layers[layers.size() - 1].calcGradients(error);
            layers[layers.size() - 2].calcWeightDeltas(grad);
            layers[layers.size() - 1].updateWeights();
            layers[layers.size() - 2].updateWeights();
        }
        // todo : move update weights to here
    }
}

void NeuralNetwork::show()
{
    for (Layer l : layers)
    {
        l.show();
    }
}

NeuralNetwork::~NeuralNetwork()
{
}
