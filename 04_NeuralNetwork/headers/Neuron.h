#include "activationFunctions.h"
#include <vector>
#include <cstdlib>
#include <ctime>

double randomDouble()
{
    // yoinked from da net
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX + 1);
}

class Neuron
{
private:
    std::vector<float> weights;
    double bias;

public:
    Neuron(int inputs);
    double forwardPass(std::vector<double> input);
    ~Neuron();
};

Neuron::Neuron(int inputs)
{
    for (int i = 0; i < inputs; i++)
    {
        weights.push_back(randomDouble());
    }
    bias = randomDouble();
}

double Neuron::forwardPass(std::vector<double> input)
{
    double output = 0.0;
    for (int i = 0; i < input.size(); i++)
    {
        output += weights[i] * input[i];
    }
    output += bias;
    return sigmoid(output);
}

Neuron::~Neuron()
{
}
