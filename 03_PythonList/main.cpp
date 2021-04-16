#include "plist.h"
#include <list>

int main(){
    plist<float> a;
    a.append(0.01f);
    a.append(0.02f);
    std::cout << a.get(0) << std::endl;
    return 0;
}