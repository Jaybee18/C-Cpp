#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

float randomFloat()
{
    // yoinked from da net
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1);
}

class Neuron
{
public:
    float bias;
    std::vector<float> weights;
    int numberOfInputs;
    float *activationFunc;

    Neuron(int pInputs, float &pActivationFunc)
    {
        bias = randomFloat();
        numberOfInputs = pInputs;
        activationFunc = pActivationFunc;

        for (int i = 0; i < pInputs; i++)
        {
            weights.push_back(randomFloat());
        }
    }

    float forwardPass(float inputs[])
    {
        int netin = 0;
        for (int i = 0; i < numberOfInputs; i++)
        {
            netin += inputs[i];
        }
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
public:
    NetworkLayer()
    {
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

float sigmoid(float x)
{
    return 1 / (1 + exp(x));
}

int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    Neuron n = Neuron(5, &sigmoid);
    n.show();
    return 0;
}