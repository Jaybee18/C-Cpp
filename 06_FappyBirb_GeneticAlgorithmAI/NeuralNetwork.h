#include "Neuron.h"
#include <vector>

class Agent
{
private:
    vector<vector<Neuron>> neurons;

public:
    Agent(int noInputs, vector<int> layers);
    vector<double> forwardPass(vector<double> inputs);
    vector<vector<Neuron>> getNeurons();
    void applyWeights(vector<double> newWeights);
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

vector<vector<Neuron>> Agent::getNeurons()
{
    return neurons;
}

void Agent::applyWeights(vector<double> newWeights)
{
    int counter = 0;
    for(vector<Neuron> &layer : neurons)
    {
        for(Neuron &n : layer)
        {
            for(int i = 0; i < n.getWeights().size(); i++)
            {
                n.setWeight(i, newWeights[counter]);
                counter++;
            }
        }
    }
}

Agent::~Agent() {}