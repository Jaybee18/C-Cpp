#include "plist.h"
#include <vector>

// todo dont implement remove by value method

int main()
{
    list<float> a;
    a.append(1);
    a.append(2);
    a.append(3);
    a.append(4);
    a.append(5);
    a.print();
    a.rotate(5);
    a.print();
    std::cout << a.pop(1) << std::endl;
    a.print();
    return 0;
}