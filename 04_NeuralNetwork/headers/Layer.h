#include "neuron.h"

class Layer
{
private:
    std::vector<Neuron> neurons;

public:
    Layer(int noNeurons, int noInputs);
    std::vector<double> forwardPass(std::vector<double> input);
    ~Layer();
};

Layer::Layer(int noNeurons, int noInputs)
{
    for (int i = 0; i < noNeurons; i++)
    {
        neurons.push_back(Neuron(noInputs));
    }
}

std::vector<double> Layer::forwardPass(std::vector<double> input)
{
    std::vector<double> output;
    for (int i = 0; i < input.size(); i++)
    {
        output.push_back(neurons[i].forwardPass(input));
    }
    return output;
}

Layer::~Layer()
{
}
