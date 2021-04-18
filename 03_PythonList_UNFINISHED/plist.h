#include <cstddef>
#include <iostream>

/* todo:
 * - implement a sort of range function
 * 
 * 
 */

template <class contentType>
struct node
{
    contentType value;
    node *next;
};

template <class contentType>
class list
{
private:
    node<contentType> *first, *last;
    int _length;

    contentType getItemAt(int index)
    {
        node<contentType> *current = first;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->value;
    }

public:
    list()
    {
        first = NULL;
        last = NULL;
        _length = 0;
    }

    void append(contentType value)
    {
        node<contentType> *temp = new node<contentType>;
        temp->value = value;
        temp->next = NULL;
        if (first == NULL)
        {
            first = temp;
            last = temp;
        }
        else
        {
            last->next = temp;
            last = temp;
        }
        temp = NULL;
        _length++;
    }

    contentType pop(int index)
    {
        if (index >= _length)
            throw std::invalid_argument("Index out of range");

        node<contentType> *current = first;
        for (int i = 0; i < index - 1; i++)
            current = first->next;

        contentType value;
        if (index == 0)
        {
            value = first->value;
            first = first->next;
        }
        else
        {
            value = current->next->value;
            current->next = current->next->next;
        }
        _length--;
        return value;
    }

    void reverse()
    {
        last = first;
        node<contentType> *current = first;
        for (int i = 0; i < _length - 1; i++)
        {
            node<contentType> *next = current->next->next;
            current->next->next = first;
            first = current->next;
            current->next = next;
        }
    }

    void rotate(int steps)
    {
        for (int i = 0; i < steps; i++)
        {
            node<contentType> *newFirst = first->next;
            node<contentType> *newLast = first;
            last->next = newLast;
            last = newLast;
            first = newFirst;
        }
    }

    list<contentType> getRange(int startIndex, int endIndex)
    {
        list<contentType> res;
        node<contentType> *current = first;
        for(int i = 0; i < _length; i++){
            if(i > endIndex)
                return res;
            if(i < startIndex)
                current = current->next;
            res.append(current->value);
            current = current->next;
        }
        
    }

    int length()
    {
        return _length;
    }

    contentType operator[](int index)
    {
        node<contentType> *current = first;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->value;
    }

    void print()
    {
        std::cout << "[ ";
        for (int i = 0; i < _length; i++)
        {
            std::cout << getItemAt(i) << " ";
        }
        std::cout << "]" << std::endl;
    }

    ~list()
    {
        delete first;
        delete last;
    }
};