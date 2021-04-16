//#include "plist.h"
#include "plist_inh.h"
#include <list>
#include <vector>

int main(){
    /*plist<float> a;
    a.append(0.01f);
    a.append(0.02f);
    std::cout << a.get(1) << std::endl;*/
    std::vector<float> a;
    a.push_back(0.1f);
    a.push_back(0.2f);
    a.push_back(0.3f);
    a.push_back(0.4f);
    std::cout << a[2] << std::endl;
    return 0;
}