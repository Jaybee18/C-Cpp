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
 * 
 * The topology parameter should be like this:
 * {neuronsInFirstLayer, neuronsInSecondLayer, ...}
 */
class Agent
{
private:
    int inputCount;
    int score;
    vector<int> topology;
    vector<vector<Neuron>> neurons;

public:
    Agent(int inputCount, vector<int> topology);
    void setScore(int newScore);
    int getScore();
    vector<double> forwardPass(vector<double> inputs);
    vector<double> getGene();
    void setGene(vector<double> gene);
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

vector<double> Agent::forwardPass(vector<double> inputs)
{
    vector<double> lastOutputs = inputs;
    for (vector<Neuron> layer : neurons)
    {
        vector<double> tempOutputs;
        for (Neuron n : layer)
            tempOutputs.push_back(n.forwardPass(lastOutputs));
        lastOutputs = tempOutputs;
    }
    return lastOutputs;
}

void Agent::setScore(int newScore)
{
    score = newScore;
}

int Agent::getScore()
{
    return score;
}

vector<double> Agent::getGene()
{
    vector<double> res;
    for (vector<Neuron> layer : neurons)
        for (Neuron n : layer)
            for (double d : n.getWeights())
                res.push_back(d);
    return res;
}

void Agent::setGene(vector<double> gene)
{
    int counter = 0;
    for (vector<Neuron> &layer : neurons)
        for (Neuron &n : layer)
            for (int i = 0; i < n.getWeights().size(); i++)
            {
                n.setWeight(i, gene[counter]);
                counter++;
            }
}

Agent::~Agent()
{
}
