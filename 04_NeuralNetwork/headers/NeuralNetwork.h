#include "Layer.h"

class NeuralNetwork
{
private:
    std::vector<Layer> layers;
    int penis;

public:
    NeuralNetwork(int noLayers, std::vector<int> noNeurons, int noInputs);
    std::vector<double> forwardPass(std::vector<double> input);
    void train(std::vector<double> input, std::vector<double> output, int iterations);
    ~NeuralNetwork();
};

NeuralNetwork::NeuralNetwork(int noLayers, std::vector<int> noNeurons, int noInputs)
{
    int lastOutputs = noInputs;
    for (int i = 0; i < noLayers; i++)
    {
        Layer temp = Layer(noNeurons[i], lastOutputs);
        layers.push_back(temp);
        lastOutputs = noNeurons[i];
    }
}

std::vector<double> NeuralNetwork::forwardPass(std::vector<double> input)
{
    std::vector<double> outputsOfLastLayer = input;
    for (int i = 0; i < input.size(); i++)
    {
        outputsOfLastLayer = layers[i].forwardPass(outputsOfLastLayer);
    }
    return outputsOfLastLayer;
}

void NeuralNetwork::train(std::vector<double> input, std::vector<double> output, int iterations)
{
    std::vector<std::vector<double>> outputs = {input};
    for (int iteration = 0; iteration < iterations; iteration++)
    {
        // forward pass
        outputs.push_back
    }

NeuralNetwork::~NeuralNetwork()
{
}
