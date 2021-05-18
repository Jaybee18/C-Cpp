#include "_GeneralHeaders/NeuralNetwork.h"
#include <iostream>

int main(){
    NeuralNetwork nn = NeuralNetwork({2, 1}, 2);
    std::cout << nn.encode() << std::endl;
    return 0;
}