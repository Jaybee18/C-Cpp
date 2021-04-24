#include "Neuron.h"

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
    void updateWeights();
    void show();
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
    for (int i = 0; i < neurons.size(); i++)
    {
        output.push_back(neurons[i].forwardPass(input));
    }
    lastOutputs = output;
    return output;
}

vector<double> Layer::calcError(vector<double> expectedOutputs)
{
    // most likely correct
    vector<double> error;
    for (int i = 0; i < expectedOutputs.size(); i++)
    {
        double temp = -2 * (expectedOutputs[i] - lastOutputs[i]);
        error.push_back(temp);
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
    // @param gradient -> ∂e/∂a_thisLayer
    // @return gradient -> ∂e/∂a_beforeLayer
    vector<vector<double>> gradients;
    for (Neuron neuron : neurons)
    {
        gradients.push_back(neuron.calcGradient(gradient));
    }
    vector<double> res;
    for (int i = 0; i < gradients[0].size(); i++)
    {
        double temp = 0.0;
        for (vector<double> grad : gradients)
        {
            temp += grad[i];
        }
        res.push_back(temp);
    }
    return res;
}

void Layer::updateWeights()
{
    // This iterates by reference ==> GOOD
    for(Neuron& n : neurons)
        n.updateWeights();
    // This iterates by value ==> MOSTLY BAD
    // for(Neuron n : neurons)
    //     n.updateWeights();
}

void Layer::show()
{
    for (Neuron n : neurons)
    {
        n.show();
    }
}

Layer::~Layer()
{
}
