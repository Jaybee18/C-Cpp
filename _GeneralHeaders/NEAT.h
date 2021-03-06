/*
 * todo : 
 * - networks are randomly generated with nodes and conenctions
 *   -> a mutation can either generate a new connection between two random nodes
 *      or generate a new node onto an existing connection
 *   -> networks who just developed a new node f.e. have a kind of baby-protection
 *      to adjust the weights of their new node so that they don't instantly die
 */

#include "Agent.h"
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

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
    NEAT(){};
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
    ~NEAT(){};
};

NEAT::NEAT(double fds)
{
    srand((unsigned int)time(NULL));
    forceDeathScore = fds;
    generation = 0;
    deadAgents = 0;
    _allDead = false;
    elitePercent = 0.1;
    mutationRate = 0.70;
}

NEAT::NEAT(double fds, double mutationRate)
{
    srand((unsigned int)time(NULL));
    forceDeathScore = fds;
    generation = 0;
    deadAgents = 0;
    _allDead = false;
    elitePercent = 0.1;
    this->mutationRate = mutationRate;
}

void NEAT::initializeAgents(int agentCount, int agentInputCount, vector<int> topology)
{
    srand((unsigned int)time(NULL));
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
    srand((unsigned int)time(NULL));
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

/*vector<Agent> sortAgentsRek(vector<Agent> agents){
    // sorting with objects like Agent takes too long!
    // instead create a list with just the scores as integers, sort it,
    // and then iterate through it while removing an agent from the agents
    // list when his score shows up
    if(agents.size() < 2)
        return agents;
    
    vector<Agent> res;
    vector<Agent> left;
    vector<Agent> right;
    int pivot = agents.size()/2;

    for(int i = 0; i < agents.size(); i++){
        if(i == pivot)
            continue;
        if(agents[i].getScore() > agents[pivot].getScore()){
            left.push_back(agents[i]);// cout << "kleiner" << endl;
        }else/* if(a.getScore() < p.getScore())
            right.push_back(agents[i]);
    }
    
    vector<Agent> sortedLeft = sortAgents(left);   
    vector<Agent> sortedRight = sortAgents(right);

    for(int i = 0; i < sortedLeft.size(); i++)
        res.push_back(sortedLeft[i]);
    res.push_back(agents[pivot]);
    for(int i = 0; i < sortedRight.size(); i++)
        res.push_back(sortedRight[i]);
    
    return res;
}*/
vector<int> sort(vector<int> liste){
    if(liste.size() < 2)
        return liste;
    
    vector<int> res;
    vector<int> left;
    vector<int> right;
    int pivot = liste.size()/2;

    for(int i = 0; i < liste.size(); i++){
        if(i == pivot)
            continue;
        if(liste[i] > liste[pivot]){
            left.push_back(liste[i]);
        }else
            right.push_back(liste[i]);
    }
    
    vector<int> sortedLeft = sort(left);   
    vector<int> sortedRight = sort(right);

    for(int i = 0; i < sortedLeft.size(); i++)
        res.push_back(sortedLeft[i]);
    res.push_back(liste[pivot]);
    for(int i = 0; i < sortedRight.size(); i++)
        res.push_back(sortedRight[i]);
    
    return res;
}

vector<Agent> sortAgents(vector<Agent> agents){
    vector<Agent> res;

    /* extract only the scores from all agents,
     * because sorting integers is way faster than
     * sorting Agent objects */
    vector<int> scores;
    for(Agent a : agents)
        scores.push_back(a.getScore());
    
    scores = sort(scores);

    /* assign the agents back to their scores */
    for(int i = 0; i < scores.size(); i++)
    {
        int score = scores[i];
        for(int j = 0; j < agents.size(); j++){
            if(agents[j].getScore() == score){
                res.push_back(agents[j]);
                agents.erase(agents.begin() + j);
                break;
            }
        }
    }

    return res;
}

Agent selectRandomAgent(vector<Agent> agents){
    /* get the total amount of points gathered by the last generation */
    int allScores = 0;
    for(Agent a : agents)
        allScores += a.getScore();
    double rand = randomDouble() * allScores;
    int counter = 0;
    while(rand > 0){
        rand -= agents[counter].getScore();
        counter++;
    }
    return agents[counter];
}

vector<Agent> selectIRandomAgents(int amount, vector<Agent> agents)
{
    int allScores = 0;
    for(Agent a : agents)
        allScores += a.getScore();
    vector<double> rands;
    for(int i = 0; i < amount; i++){
        rands.push_back((rand()/(RAND_MAX*1.0)) * allScores);
    }
    vector<Agent> res;
    int counter = 0;
    while(res.size() <= amount){
        if(counter >= agents.size())
            break;
        for(double &d : rands){
            if(d <= 0)
                continue;
            d -= agents[counter].getScore();
            if(d <= 0)
                res.push_back(agents[counter]);
        }
        counter++;
    }
    return res;
}

void NEAT::generateNewGeneration()
{
    deadAgents = 0;
    _allDead = false;
    /* sort all agents by score */
    vector<Agent> tempAgents = sortAgents(agents);

    std::cout << "Generation : " << generation << " | Score : " << tempAgents[0].getScore() << std::endl;
    generation++;

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
    int amountOfChildrenPerPair = 2;
    iterations = amount_of_agents - iterations;
    int numberOfPairs = iterations / amountOfChildrenPerPair;
    vector<Agent> newGen;
    vector<Agent> randomAgents = selectIRandomAgents(numberOfPairs*2, agents);

    for(int i = 0; i < numberOfPairs*2; i+=2)
    {
        Agent a1 = randomAgents[i], a2 = randomAgents[i+1];
        vector<double> tempGene = generateNewGene(a1, a2);
        for(Agent a : generateAgentsFromGene(amountOfChildrenPerPair, tempGene))
            newGen.push_back(a);
    }
    for (Agent a : elite)
        newGen.push_back(a);

    /* replace the old generation with the newer */
    agents = newGen;
}

void NEAT::killAgent(int agentIndex)
{
    if(isAgentDead(agentIndex))
        return;
    agents[agentIndex].kill();
    deadAgents++;
    if (deadAgents == amount_of_agents)
        _allDead = true;
}

bool NEAT::allDead()
{
    return _allDead;
}

vector<double> NEAT::generateNewGene(Agent agent1, Agent agent2)
{
    vector<double> gene1 = agent1.getGene();
    vector<double> gene2 = agent2.getGene();

    /* just testing */
    double prob1 = agent1.getScore();
    double prob2 = agent2.getScore();

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