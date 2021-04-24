#include "Neuron.h"
#include <vector>

class Agent
{
private:
    vector<vector<Neuron>> neurons;

public:
    Agent(int noInputs, vector<int> layers);
    vector<double> forwardPass(vector<double> inputs);
    ~Agent();
};

Agent::Agent(int noInputs, vector<int> layers)
{
    int lastInputs = noInputs;
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        vector<Neuron> temp;
        for (int j = 0; j < layers[i]; j++)
        {
            temp.push_back(Neuron(lastInputs));
        }
        neurons.push_back(temp);
        lastInputs = layers[i];
    }
}

vector<double> Agent::forwardPass(vector<double> inputs)
{
    vector<double> lastOutputs = inputs;
    for (int layerIndex = 0; layerIndex < neurons.size(); layerIndex++)
    {
        vector<double> temp;
        for (Neuron &n : neurons[layerIndex])
        {
            temp.push_back(n.forwardPass(lastOutputs));
        }
        lastOutputs = temp;
    }
    return lastOutputs;
}

Agent::~Agent() {}