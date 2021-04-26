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

vector<double> generateNewGene(vector<double> gene1, vector<double> gene2, int agent1, int agent2)
{
    agent1 *= agent1;
    agent2 *= agent2;
    double prob1 = agent1/(agent1 + agent2), prob2 = agent2/(agent1 + agent2);
    vector<double> res;
    for(int i = 0; i < gene1.size(); i++)
    {
        /* natural evolution */
        if(rand()/(RAND_MAX*1.0) >= prob1)
        {
            res.push_back(gene1[i]);
        }else
        {
            res.push_back(gene2[i]);
        }
    }
    return res;
}

vector<double> mutate(vector<double> gene){
    vector<double> res;
    for(int i = 0; i < gene.size(); i++){
        if(rand()/(RAND_MAX*1.0) >= 0.98)
            res.push_back(rand()/(RAND_MAX/2.0)-1);
        else
            res.push_back(gene[i]);
    }
    return res;
}

vector<Agent> generateAgentsFromGene(int noAgents, vector<double> gene, vector<int> agentNetworkTopology)
{
    vector<Agent> res;
    int agentInputs = 2;
    for(int i = 0; i < noAgents; i++){
        Agent temp = Agent(agentInputs, agentNetworkTopology); // todo : dynamic
        temp.applyWeights(mutate(gene));
        res.push_back(temp);
    }
    return res;
}