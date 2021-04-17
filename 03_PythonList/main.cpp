#include "plist.h"
#include <vector>

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