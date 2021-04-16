#include "plist.h"
#include <list>

int main(){
    plist<float> a;
    a.append(0.01f);
    a.append(0.02f);
    return 0;
}