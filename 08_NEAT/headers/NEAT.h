#include "Agent.h"
#include <vector>

using std::vector;

class NEAT
{
private:
    int amount_of_agents;
    int generation;
    int agentInputs;
    double elitePercent;
    double mutationRate;
    vector<int> networkTopology;
    vector<Agent> agents;
    vector<double> generateNewGene(Agent agent1, Agent agent2);
    vector<double> mutate(vector<double> gene);
    vector<Agent> generateAgentsFromGene(int noAgents, vector<double> gene);

public:
    NEAT();
    NEAT(double mutationRate);
    void initializeAgents(int agentCount, int agentInputCount, vector<int> topology);
    void initializeAgents(int agentCount, int agentInputCount, vector<int> topology, double elitePercent);
    void setScoreOfAgent(int agentIndex, int newScore);
    vector<vector<double>> getOutput(vector<vector<double>> inputs);
    void generateNewGeneration();
    ~NEAT();
};

NEAT::NEAT()
{
    elitePercent = 0.05;
    mutationRate = 0.70;
}

NEAT::NEAT(double mutationRate)
{
    elitePercent = 0.05;
    this->mutationRate = mutationRate;
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology)
{
    agentInputs = agentInputCount;
    amount_of_agents = agentCount;
    networkTopology = topology;
    for (int i = 0; i < amount_of_agents; i++)
        agents.push_back(Agent(agentInputCount, topology));
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology, double elitePercent)
{
    agentInputs = agentInputCount;
    this->elitePercent = elitePercent;
    amount_of_agents = agentCount;
    networkTopology = topology;
    for (int i = 0; i < amount_of_agents; i++)
        agents.push_back(Agent(agentInputCount, topology));
}

void NEAT::setScoreOfAgent(int agentIndex, int newScore)
{
    agents[agentIndex].setScore(newScore);
}

vector<vector<double>> NEAT::getOutput(vector<vector<double>> inputs)
{
    vector<vector<double>> outputs;
    for (int i = 0; i < inputs.size(); i++)
        outputs.push_back(agents[i].forwardPass(inputs[i]));
    return outputs;
}

void NEAT::generateNewGeneration()
{
    /* sort all agents by score */
    bool switched = true;
    while (switched)
    {
        switched = false;
        for (int i = 0; i < agents.size() - 1; i++)
        {
            if (agents[i].getScore() < agents[i + 1].getScore())
            {
                Agent temp = agents[i];
                agents[i] = agents[i + 1];
                agents[i + 1] = temp;
                switched = true;
            }
        }
    }

    /* isolate the elite */
    int iterations = agents.size() * elitePercent;
    vector<Agent> elite;
    for (int i = 0; i < iterations; i++)
        elite.push_back(agents[i]);

    /* generate the rest of the new generation */
    vector<double> newGene = generateNewGene(elite[0], elite[1]);
    iterations = amount_of_agents - iterations;
    vector<Agent> newGen = generateAgentsFromGene(iterations, newGene);
    for(Agent a : elite)
        newGen.push_back(a);
    
    /* replace the old generation with the newer */
    agents = newGen;
}

NEAT::~NEAT()
{
}

vector<double> NEAT::generateNewGene(Agent agent1, Agent agent2)
{
    vector<double> gene1 = agent1.getGene();
    vector<double> gene2 = agent2.getGene();

    /* just testing */
    double prob1 = 0.5;
    double prob2 = 0.5;

    vector<double> res;
    for (int i = 0; i < gene1.size(); i++)
    {
        /* natural evolution */
        if (rand() / (RAND_MAX * 1.0) >= prob1)
        {
            res.push_back(gene1[i]);
        }
        else
        {
            res.push_back(gene2[i]);
        }
    }
    return res;
}

vector<double> NEAT::mutate(vector<double> gene)
{
    vector<double> res;
    for (int i = 0; i < gene.size(); i++)
    {
        if (rand() / (RAND_MAX * 1.0) >= mutationRate)
            res.push_back(rand() / (RAND_MAX / 2.0) - 1);
        else
            res.push_back(gene[i]);
    }
    return res;
}

vector<Agent> NEAT::generateAgentsFromGene(int noAgents, vector<double> gene){
    vector<Agent> res;
    for (int i = 0; i < noAgents; i++)
    {
        Agent temp = Agent(agentInputs, networkTopology);
        temp.setGene(mutate(gene));
        res.push_back(temp);
    }
    return res;
}