#include "plist.h"
#include <vector>

int main()
{
    list<float> a;
    a.append(0.01f);
    a.append(0.02f);
    a.append(0.03f);
    a.print();
    a.remove(0.2f);
    a.print();
    return 0;
}