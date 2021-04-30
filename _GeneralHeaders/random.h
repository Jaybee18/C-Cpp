#include <cstdlib>
#include <ctime>

class RandomDoubleGenerator
{
public:
    RandomDoubleGenerator();
    double generate();
};

RandomDoubleGenerator::RandomDoubleGenerator()
{
    srand((unsigned int)time(NULL));
}

double RandomDoubleGenerator::generate()
{
    return rand() / (RAND_MAX * 1.0);
}