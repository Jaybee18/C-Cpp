#include "plist.h"
#include <vector>

int main()
{
    list<int> a; // float has problems for some reason
    a.append(1);
    a.append(2);
    a.append(3);
    a.print();
    a.remove(2);
    a.print();
    return 0;
}