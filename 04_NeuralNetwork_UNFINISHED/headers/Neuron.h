#include "activationFunctions.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream> // debugging

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
    vector<double> calcGradient(vector<double> gradient);
    void updateWeights();
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
    return sigmoid(output);
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
    vector<double> placeholder;
    for (int i = 0; i < lastIn.size(); i++)
    {
        double temp = gradient * sigmoid_d(lastNetIn) * lastIn[i];
        //std::cout << temp << std::endl;
        placeholder.push_back(temp);
        weights[i] -= temp;
    }
    delta = placeholder;
    //bias can be updated instantly
    //bias -= gradient * sigmoid_d(lastNetIn) * 1;
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
    cout << "fuck my life" << endl;
    double ficken = 100.0;
    weights = {100.0, 90.0};
}

void Neuron::show()
{
    cout << endl << "Neuron" << endl;
    for (double w : weights)
    {
        cout << w << endl;
    }
    cout << endl;
}

Neuron::~Neuron()
{
}
