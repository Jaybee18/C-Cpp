#include "NeuralNetwork.h"
#include <vector>

vector<double> extractWeights(Agent agent)
{
    vector<double> res;
    vector<vector<Neuron>> neurons = agent.getNeurons();
    for(vector<Neuron> layer : neurons)
    {
        for(Neuron n : layer)
        {
            for(double d : n.getWeights())
            {
                res.push_back(d);
            }
        }
    }
    return res;
}

vector<double> generateNewGene(vector<double> gene1, vector<double> gene2)
{
    vector<double> res;
    for(int i = 0; i < gene1.size(); i++)
    {
        /* mutation */
        if(rand()/(RAND_MAX*1.0) >= 0.8)
        {
            res.push_back(rand()/15000.0-1);
            continue;
        }

        /* natural evolution */
        if(rand()/(RAND_MAX*1.0) >= 0.5)
        {
            res.push_back(gene1[i]);
        }else
        {
            res.push_back(gene2[i]);
        }
    }
    return res;
}

vector<Agent> generateAgentsFromGene(int noAgents, vector<double> gene)
{
    vector<Agent> res;
    for(int i = 0; i < noAgents; i++){
        Agent temp = Agent(3, {4, 4, 1}); // todo : dynamic
        temp.applyWeights(gene);
        res.push_back(temp);
    }
    return res;
}