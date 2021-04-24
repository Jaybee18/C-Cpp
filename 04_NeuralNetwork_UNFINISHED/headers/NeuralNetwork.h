#include "Layer.h"
#include <iostream>

using namespace std;

class NeuralNetwork
{
private:
    vector<Layer> layers;

public:
    NeuralNetwork(int noLayers, vector<int> noNeurons, int noInputs);
    vector<double> forwardPass(vector<double> input);
    void train(vector<double> input, vector<double> output, int iterations);
    void show();
    ~NeuralNetwork();
};

NeuralNetwork::NeuralNetwork(int noLayers, vector<int> noNeurons, int noInputs)
{
    int lastOutputs = noInputs;
    for (int i = 0; i < noLayers; i++)
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

void NeuralNetwork::train(vector<double> input, vector<double> output, int iterations)
{
    for (int iteration = 0; iteration < iterations; iteration++)
    {
        // forward pass
        vector<vector<double>> outputs;
        vector<double> lastOutput = input;
        for (int i = 0; i < layers.size(); i++)
        {
            vector<double> tempOutput = layers[i].forwardPass(input);
            outputs.push_back(tempOutput);
            lastOutput = tempOutput;
        }

        // backpropagation
        /* calculate the mse wrt each of the activations */
        vector<double> error = layers[layers.size() - 1].calcError(output);

        layers[layers.size() - 1].calcWeightDeltas(error);

        layers[layers.size() - 1].updateWeights();
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
