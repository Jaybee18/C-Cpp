#include "plist.h"
#include <vector>

int main()
{
    list<float> a = {1, 2, 3};
    a[1]->value = 3;
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
    list<float> temp = a.getRange(1, 3);
    temp.print();
    temp.ljust(6, 0);
    temp.print();
    temp.rjust(9, 1);
    temp.print();
    a.extend(&temp);
    a.print();
    a.insert(1, 13);
    a.print();
    list<int> test = {1, 2, 3};
    test.print();
    return 0;
}