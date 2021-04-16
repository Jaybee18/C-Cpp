#include <cstddef>
#include <iostream>

template <class contentType>
class obj
{
public:
    contentType value;
    obj<contentType>* next;
    bool isEmpty = true;
    obj() {}
    obj(contentType pValue)
    {
        value = pValue;
        isEmpty = true;
    }
};

template <class contentType>
class plist
{
public:
    obj<contentType> first;
    obj<contentType> current;
    plist()
    {
    }

    void toFirst()
    {
        current = first;
    }

    void next()
    {
        current = *(current.next);
    }

    void append(contentType object)
    {
        if (first.isEmpty)
        {
            first = obj<contentType>(object);
            return;
        }
        toFirst();
        int safetyCounter = 0;
        while (!(*current.next).isEmpty)
        {
            if( safetyCounter > 100 ){std::cout << "while true loop!" << std::endl; break;}
            safetyCounter++;
            next();
        }
        current.next = new obj<contentType>(object);
    }
};