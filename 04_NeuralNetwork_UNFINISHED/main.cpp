#include "headers/NeuralNetwork.h"
#include <iostream>

int main()
{
    NeuralNetwork n = NeuralNetwork(1, {1}, 2);

    std::vector<double> output = n.forwardPass({1.0, 0.0});
    for (int i = 0; i < output.size(); i++)
    {
        std::cout << "Output Neuron #" << i << " : " << output[i] << std::endl;
    }
    n.train({1.0, 0.0}, {1.0}, 1);

    output = n.forwardPass({1.0, 0.0});
    for (int i = 0; i < output.size(); i++)
    {
        std::cout << "Output Neuron #" << i << " : " << output[i] << std::endl;
    }
    return 0;
}