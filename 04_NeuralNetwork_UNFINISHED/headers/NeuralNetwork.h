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
    vector<double> outputsOfLastLayer = input;
    for (int i = 0; i < input.size(); i++)
    {
        outputsOfLastLayer = layers[i].forwardPass(outputsOfLastLayer);
    }
    return outputsOfLastLayer;
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

        /* calculate weight deltas based on error */
        vector<double> lastGradient = error;
        for (Layer layer : layers)
        {
            /*
             * It has to be calculated ∂e/∂w for each neuron in each layer                                                      ∑
             * ∂e/∂w = ∂e/∂a * ∂a/∂z * ∂z/∂w
             *           ||   |____________|
             *           ||         \/
             * the neuron itself has these variables, so they don't need to be passed
             *           ||
             *           \/
             * has to be passed bc it contains the error
             */
            layer.calcWeightDeltas(lastGradient);               // ∂e/∂w
            lastGradient = layer.calcGradients(lastGradient);   // ∂e/∂a
        }

        /* update all the weights */
        for(Layer layer : layers){
            //layer.updateWeights();
        }
    }
}

NeuralNetwork::~NeuralNetwork()
{
}
