#include "plist.h"
#include <vector>

// todo dont implement remove by value method

int main()
{
    list<float> a;
    a.append(1);
    a.append(2);
    a.append(3);
    a.print();
    a.remove(2);
    a.print();
    return 0;
}