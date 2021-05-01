#include "headers/NEAT.h"

int main(){
    NEAT neat = NEAT();
    neat.initializeAgents(500, 3, {6, 1});
    return 0;
}