#include "Agent.h"
#include <vector>

using std::vector;

class NEAT
{
private:
    int amount_of_agents;
    int generation;
    int agentInputs;
    int deadAgents;
    int forceDeathScore;
    bool _allDead;
    double elitePercent;
    double mutationRate;
    vector<int> networkTopology;
    vector<Agent> agents;
    vector<double> generateNewGene(Agent agent1, Agent agent2);
    vector<double> mutate(vector<double> gene);
    vector<Agent> generateAgentsFromGene(int noAgents, vector<double> gene);

public:
    NEAT();
    NEAT(double fds);
    NEAT(double fds, double mutationRate);
    void initializeAgents(int agentCount, int agentInputCount, vector<int> topology);
    void initializeAgents(int agentCount, int agentInputCount, vector<int> topology, double elitePercent);
    void setScoreOfAgent(int agentIndex, int newScore);
    vector<vector<double>> getOutput(vector<vector<double>> inputs);
    void generateNewGeneration();
    void killAgent(int agentIndex);
    bool allDead();
    bool isAgentDead(int agentIndex);
    void increaseScore(int agentIndex);
    vector<double> getSingleAgentOutput(int agentIndex, vector<double> input);
    ~NEAT();
};

NEAT::NEAT() {}

NEAT::NEAT(double fds)
{
    forceDeathScore = fds;
    deadAgents = 0;
    _allDead = false;
    elitePercent = 0.05;
    mutationRate = 0.70;
}

NEAT::NEAT(double fds, double mutationRate)
{
    forceDeathScore = fds;
    deadAgents = 0;
    _allDead = false;
    elitePercent = 0.05;
    this->mutationRate = mutationRate;
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology)
{
    _allDead = false;
    deadAgents = 0;
    agentInputs = agentInputCount;
    amount_of_agents = agentCount;
    networkTopology = topology;
    for (int i = 0; i < amount_of_agents; i++)
        agents.push_back(Agent(agentInputCount, topology));
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology, double elitePercent)
{
    _allDead = false;
    deadAgents = 0;
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
    return;
    deadAgents = 0;
    _allDead = false;
    /* sort all agents by score */
    vector<Agent> tempAgents = agents;
    bool switched = true;
    while (switched)
    {
        switched = false;
        for (int i = 0; i < tempAgents.size() - 1; i++)
        {
            if (tempAgents[i].getScore() < tempAgents[i + 1].getScore())
            {
                Agent temp = tempAgents[i];
                tempAgents[i] = tempAgents[i + 1];
                tempAgents[i + 1] = temp;
                switched = true;
            }
        }
    }

    /* isolate the elite */
    int iterations = tempAgents.size() * elitePercent;
    vector<Agent> elite;
    if (iterations < 2)
    {
        tempAgents[0].reset();
        elite.push_back(tempAgents[0]);
        elite.push_back(tempAgents[0]);
    }
    else
    {
        for (int i = 0; i < iterations; i++)
        {
            tempAgents[i].reset();
            elite.push_back(tempAgents[i]);
        }
    }

    /* generate the rest of the new generation */
    vector<double> newGene = generateNewGene(elite[0], elite[1]);
    iterations = amount_of_agents - iterations;
    vector<Agent> newGen = generateAgentsFromGene(iterations, newGene);
    for (Agent a : elite)
        newGen.push_back(a);

    /* replace the old generation with the newer */
    agents = newGen;
}

void NEAT::killAgent(int agentIndex)
{
    agents[agentIndex].kill();
    deadAgents++;
    if (deadAgents == amount_of_agents)
        _allDead = true;
}

bool NEAT::allDead()
{
    return _allDead;
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

vector<Agent> NEAT::generateAgentsFromGene(int noAgents, vector<double> gene)
{
    vector<Agent> res;
    for (int i = 0; i < noAgents; i++)
    {
        Agent temp = Agent(agentInputs, networkTopology);
        vector<double> mutatedGene = mutate(gene);
        temp.setGene(mutatedGene);
        res.push_back(temp);
    }
    return res;
}

bool NEAT::isAgentDead(int agentIndex) { return agents[agentIndex].isDead; }

void NEAT::increaseScore(int agentIndex)
{
    agents[agentIndex].setScore(agents[agentIndex].getScore() + 1);
    if (agents[agentIndex].getScore() >= forceDeathScore)
        killAgent(agentIndex);
}

vector<double> NEAT::getSingleAgentOutput(int agentIndex, vector<double> input)
{
    return agents[agentIndex].forwardPass(input);
}