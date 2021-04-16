#include <list>
#include <iostream>

template <class contentType>
class plist : public std::list<contentType>{
    public:
    plist(){
        std::cout << "constructor called" << std::endl;
    }
};