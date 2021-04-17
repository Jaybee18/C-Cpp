#include "headers/NeuralNetwork.h"
#include <iostream>

int main()
{
    std::vector<int> noNeurons = {2, 2};
    NeuralNetwork n = NeuralNetwork(2, noNeurons, 2);

    std::vector<double> input = {1.0, 0.0};
    std::vector<double> output = n.forwardPass(input);
    for (int i = 0; i < output.size(); i++)
    {
        std::cout << "Output Neuron #" << i << " : " << output[i] << std::endl;
    }
    return 0;
}