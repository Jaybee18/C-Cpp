#include "activationFunctions.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

double randomDouble()
{
    // yoinked from da net
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX + 1);
}

class Neuron
{
private:
    vector<double> weights;
    vector<double> lastIn;
    double bias;
    vector<double> delta;
    double lastNetIn;

public:
    Neuron(int inputs);
    double forwardPass(vector<double> input);
    void calcDelta(double gradient);
    double calcGradient(vector<double> gradient);
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

double Neuron::forwardPass(vector<double> input)
{
    double output = 0.0;
    for (int i = 0; i < input.size(); i++)
    {
        output += weights[i] * input[i];
    }
    output += bias;
    return sigmoid(output);
}

void Neuron::calcDelta(double gradient)
{
    for (int i = 0; i < lastIn.size(); i++)
    {
        delta[i] = gradient * sigmoid_d(lastNetIn) * lastIn[i];
    }
}

vector<double> Neuron::calcGradient(vector<double> gradient)
{
    vector<double> gradient;
    
}

Neuron::~Neuron()
{
}
