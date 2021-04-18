#include "neuron.h"

using namespace std;

class Layer
{
private:
    vector<Neuron> neurons;
    vector<double> lastOutputs;

public:
    Layer(int noNeurons, int noInputs);
    vector<double> forwardPass(vector<double> input);
    vector<double> calcError(vector<double> expectedOutput);
    void calcWeightDeltas(vector<double> gradient);
    vector<double> calcGradients(vector<double> gradient);
    ~Layer();
};

Layer::Layer(int noNeurons, int noInputs)
{
    for (int i = 0; i < noNeurons; i++)
    {
        neurons.push_back(Neuron(noInputs));
    }
}

vector<double> Layer::forwardPass(vector<double> input)
{
    vector<double> output;
    for (int i = 0; i < input.size(); i++)
    {
        output.push_back(neurons[i].forwardPass(input));
    }
    lastOutputs = output;
    return output;
}

vector<double> Layer::calcError(vector<double> expectedOutputs)
{
    vector<double> error;
    for (int i = 0; i < expectedOutputs.size(); i++)
    {
        error.push_back(-2 * (expectedOutputs[i] - lastOutputs[i]));
    }
    return error;
}

void Layer::calcWeightDeltas(vector<double> gradient)
{
    for (int i = 0; i < neurons.size(); i++)
    {
        neurons[i].calcDelta(gradient[i]);
    }
}

vector<double> Layer::calcGradients(vector<double> gradient)
{
    vector<double> res;
    for(int i = 0; i < neurons.size(); i++){
        res.push_back(neurons[i].calcGradient(gradient));
    }
}

Layer::~Layer()
{
}
