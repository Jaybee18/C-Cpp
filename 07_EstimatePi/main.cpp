#include "C:\Users\Jan\Desktop\Programmieren\C, CPP\C-CPP\_GeneralHeaders\plist.h"
#include "C:\Users\Jan\Desktop\Programmieren\C, CPP\C-CPP\_GeneralHeaders\random.h"
#include <math.h>
#include <iostream>

RandomDoubleGenerator randgen = RandomDoubleGenerator();
static int amount_of_points = 100000000;
static double squareWidth = 2.0;
int points_in_circle = 0;

int main()
{
    for (int i = 0; i < amount_of_points; i++)
    {
        double p[2] = {randgen.generate()*2-1, randgen.generate()*2-1};

        /* check whether or not the generated point
         * is in the inner circle */
        double distance_to_center = sqrt(pow(p[0], 2) + pow(p[1], 2));
        if (distance_to_center < 1.0)
            points_in_circle++;
    }
    double finalValue = (points_in_circle*1.0/amount_of_points*1.0*4.0);
    std::cout << finalValue << std::endl;
    return 0;
}