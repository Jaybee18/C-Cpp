#include "activationFunctions.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

/* generate a random double between
 * -1.0 and 1.0 */
double randomDouble()
{
    return rand()/(RAND_MAX/2.0)-1;
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
    vector<double> calcGradient(vector<double> gradient);
    void updateWeights();
    vector<double> getWeights();
    void setWeight(int index, double w);
    void show();
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
    lastIn = input;
    double output = 0.0;
    for (int i = 0; i < input.size(); i++)
    {
        output += weights[i] * input[i];
    }
    output += bias;
    lastNetIn = output;
    return relu(output);
}

/*
 * calculates the weight deltas for this neuron:
 * 
 * ∂e/∂w = ∂e/∂a * ∂a/∂z * ∂z/∂w
 * 
 * with ∂e/∂a being @gradient.
 * all the other values are stored locally
 * and don't need to be passed.
 */
void Neuron::calcDelta(double gradient)
{
    delta.clear();
    for (int i = 0; i < lastIn.size(); i++)
    {
        double temp = gradient * sigmoid_d(lastNetIn) * lastIn[i];
        delta.push_back(temp);
    }
    bias -= gradient * sigmoid_d(lastNetIn) * 1;
}

vector<double> Neuron::calcGradient(vector<double> gradient)
{
    vector<double> res;
    for (int i = 0; i < gradient.size(); i++)
    {
        res.push_back(gradient[i] * sigmoid_d(lastNetIn) * weights[i]);
    }
    return res;
}

void Neuron::updateWeights()
{
    for (int i = 0; i < weights.size(); i++)
        weights[i] -= delta[i];
}

vector<double> Neuron::getWeights()
{
    return weights;
}

void Neuron::setWeight(int index, double w)
{
    weights[index] = w;
}

Neuron::~Neuron()
{
}
