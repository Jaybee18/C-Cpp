#include <math.h>

double sigmoid(double x)
{
    return 1 / (1 + exp(x));
}

double sigmoid_d(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}

double relu(double x)
{
    return x > 0 ? x : 0;
}

double relu_d(double x)
{
    return x > 0 ? 1 : 0;
}