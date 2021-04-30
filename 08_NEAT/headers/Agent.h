#include "Neuron.h"
#include <vector>

using std::vector;

/* An Agent is a Neural Network
 * that is able to give outputs
 * based on inputs (like a normal neural network)
 * but can also extract it's weights and 
 * biases so that they can be evolved
 * and crossed with others. The new so
 * called genes can then be applied by 
 * the Agent.
 */
class Agent
{
private:
    int inputCount;
    vector<int> topology;
    vector<vector<Neuron>> neurons;

public:
    Agent(int inputCount, vector<int> topology);
    ~Agent();
};

Agent::Agent(int inputCount, vector<int> topology)
{
    this->inputCount = inputCount;
    this->topology = topology;

    int lastInputs = inputCount;
    for (int layerIndex = 0; layerIndex < topology.size(); layerIndex++)
    {
        vector<Neuron> layer;
        for (int neuronIndex = 0; neuronIndex < topology[layerIndex]; neuronIndex++)
        {
            layer.push_back(Neuron(lastInputs));
        }
        neurons.push_back(layer);
        lastInputs = topology[layerIndex];
    }
}

Agent::~Agent()
{
}
