#include "Neuron.h"
#include <vector>

class NeuralNetwork
{
private:
    vector<vector<Neuron>> neurons;
    int inputs, outputs;
public:
    NeuralNetwork(){};
    NeuralNetwork(vector<int> topology, int inputs);
    vector<double> forwardPass(vector<double> inputs);
    string encode();
    void decode(string str);
    vector<double> getWeights();
    void setWeights(vector<double> weights);
    ~NeuralNetwork(){};
};

NeuralNetwork::NeuralNetwork(vector<int> topology, int inputs)
{
    this->inputs = inputs;
    this->outputs = topology[topology.size()-1];

    int lastInputs = inputs;
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

vector<double> NeuralNetwork::forwardPass(vector<double> inputs)
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

vector<double> NeuralNetwork::getWeights()
{
    vector<double> res;
    for (vector<Neuron> layer : neurons)
        for (Neuron n : layer)
            for (double d : n.getWeights())
                res.push_back(d);
    return res;
}

void NeuralNetwork::setWeights(vector<double> weights){
    int counter = 0;
    for (vector<Neuron> &layer : neurons)
    {
        for (Neuron &n : layer)
        {
            for (int i = 0; i < n.getWeights().size()-1; i++)
            {
                n.setWeight(i, weights[counter]);
                counter++;
            }
            n.setBias(weights[counter]);
            counter++;
        }
    }
}

/* //Encodes the entire NeuralNetwork into one string
 * //the swappable components of the "gene" are seperated by
 * //semicolons, so that any network can exchange parts of
 * //their genes as long as the parts were in the same position.
 * 
 * Format:
 * //inputs;listOfNeuronsInEachLayer;weightb;weight1;...;weightInputs;2weightb;2weight1;...;2weightInputs;...
 * start;inputs;neuron1;neuron2;...;connection1;connection2;...;o;outputNeuron1;...;end;
 *
 */
string NeuralNetwork::encode()
{
    string gene = "start;";

    /* inputs */
    gene.append(to_string(inputs) + ";");

    /* neurons and connections*/
    int counter = 0;
    for(vector<Neuron> nn : neurons)
        for(Neuron n : nn){
            gene.append("n" + to_string(counter) + ";");
            counter++;
            
        }
    return gene;
}