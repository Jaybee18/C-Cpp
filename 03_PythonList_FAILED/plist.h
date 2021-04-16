#include <cstddef>
#include <iostream>

template <class contentType>
struct obj
{
public:
    contentType value;
    obj<contentType>* next;
    bool isEmpty = true;
    obj()
    {
        next = (obj<contentType> *)malloc(sizeof(obj<contentType>));
        //next = nullptr;
    }
    obj(contentType pValue)
    {
        value = pValue;
        isEmpty = false;
        next = (obj<contentType> *)malloc(sizeof(obj<contentType>));
        //next = nullptr;
    }
    void addNext(contentType val){
        next = new obj<contentType>(val);
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
    ~plist()
    {
    }

    void toFirst()
    {
        current = first;
    }

    void next()
    {
        std::cout << "value of current before next() : " << current.value << std::endl;
        current = *(current.next);
        std::cout << "value of current after next() : " << current.value << std::endl;
    }

    contentType get(int index)
    {
        toFirst();
        std::cout << (current.next)->value << std::endl;
        for (int i = 0; i < index; i++)
        {
            next();
        }
        return current.value;
    }

    void append(contentType object)
    {
        //don't touch! @Jaybee18
        if (first.isEmpty)
        {
            first = *(new obj<contentType>(object));
            return;
        }
        toFirst();
        int safetyCounter = 0;
        while (current.isEmpty)
        {
            if (safetyCounter > 100)
            {
                std::cout << "while true loop!" << std::endl;
                break;
            }
            safetyCounter++;
            next();
        }
        current.addNext(object);
        std::cout << "variable was set with value of : " << current.next->value << std::endl;
        return;
    }
};