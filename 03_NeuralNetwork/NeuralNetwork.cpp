#include <iostream>
#include <random>

float randomFloat()
{
    // yoinked from da net
    std::default_random_engine e;
    std::uniform_real_distribution<> dist(0, 1);
    return dist(e);
}

class Neuron
{
public:
    float bias;
    float *weights;
    float weight[3];
    int inputs;

    Neuron(int pInputs)
    {
        bias = randomFloat();
        inputs = pInputs;

        float temp[pInputs];
        for (int i = 0; i < pInputs; i++)
        {
            temp[i] = randomFloat();
        }
        weights = temp;
        std::cout << weights[4] << std::endl;
    }

    void show()
    {
        std::cout << "---------------" << std::endl;
        std::cout << "Weights :" << std::endl;
        for (int i = 0; i < inputs; i++)
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

int main()
{
    Neuron n = Neuron(5);
    n.show();
    return 0;
}