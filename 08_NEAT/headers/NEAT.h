#include "Agent.h"
#include <vector>

using std::vector;

class NEAT
{
private:
    int amount_of_agents;
    int generation;
    vector<int> networkTopology;
    vector<Agent> agents;

public:
    NEAT();
    void initializeAgents(int agentCount, int agentInputCount, vector<int> topology);
    ~NEAT();
};

NEAT::NEAT()
{
    amount_of_agents = -1;
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology)
{
    amount_of_agents = agentCount;
    networkTopology = topology;
    for (int i = 0; i < amount_of_agents; i++)
    {
        agents.push_back(Agent(agentInputCount, topology));
    }
}

NEAT::~NEAT()
{
}
