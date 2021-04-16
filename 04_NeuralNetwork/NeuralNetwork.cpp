#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>

float sigmoid(float x)
{
    return 1 / (1 + exp(x));
}

float randomFloat()
{
    // yoinked from da net
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1);
}

class Neuron
{
private:
    float bias;
    std::vector<float> weights;
    int numberOfInputs;
    float lastNetIn;

public:
    Neuron(int pInputs)
    {
        bias = randomFloat();
        numberOfInputs = pInputs;

        for (int i = 0; i < pInputs; i++)
        {
            weights.push_back(randomFloat());
        }
    }

    float forwardPass(float inputs[])
    {
        float netin = 0;
        for (int i = 0; i < numberOfInputs; i++)
        {
            netin += inputs[i] * weights[i];
        }
        lastNetIn = netin;
        return sigmoid(netin);
    }

    float getWeightAtIndex(int index)
    {
        return weights[index];
    }

    void adjustWeightAtIndex(int index, float delta)
    {
        weights[index] -= delta;
    }

    void adjustBiasWeight(float delta)
    {
        bias -= delta;
    }

    void show()
    {
        std::cout << "---------------" << std::endl;
        std::cout << "Weights :" << std::endl;
        for (int i = 0; i < numberOfInputs; i++)
        {
            std::cout << i << " : " << weights[i] << std::endl;
        }
        std::cout << "Bias : " << bias << std::endl;
        std::cout << "---------------" << std::endl;
    }
};

class NetworkLayer
{
private:
    std::vector<Neuron> neurons;
    int numberOfInputs;
    int numberOfNeurons;
    float lastInputs[];
public:
    NetworkLayer(int pNumberOfInputs, int pNumberOfNeurons)
    {
        numberOfInputs = pNumberOfInputs;
        numberOfNeurons = pNumberOfNeurons;
        for(int i = 0; i < pNumberOfInputs; i++){
            neurons.push_back(Neuron(numberOfInputs));
        }
    }

    float forwardPass(float inputs[]){

    }
};

class NeuralNetwork
{
public:
    int layers;

    NeuralNetwork(int l)
    {
        layers = l;
    }
};

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    Neuron n = Neuron(2);
    n.show();

    float inputs[2] = {1.0f, 0.0f};
    std::cout << n.forwardPass(inputs) << std::endl;

    return 0;
}