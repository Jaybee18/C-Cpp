#include <cstddef>
#include <iostream>

/* todo:
 * - sort
 * - 
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

    // todo : rework, this is bad
    void insert(contentType value, int index)
    {
        if (index > _length)
            throw std::invalid_argument("Index out of range");
        node<contentType> *newObject = new node<contentType>;
        newObject->value = value;
        if(index == 0){
            newObject->next = first;
            first = newObject;
            _length++;
            return;
        }
        if(index == _length){
            newObject->next = NULL;
            last->next = newObject;
            last = newObject;
            _length++;
            return;
        }
        node<contentType> *current = first;
        for (int i = 0; i < index - 1; i++)
            current = current->next;

        newObject->next = current->next;
        current->next = newObject;

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
        for (int i = 0; i < _length; i++)
        {
            if (i >= endIndex)
                return res;
            if (i < startIndex)
                current = current->next;
            res.append(current->value);
            current = current->next;
        }
        current = NULL;
        return res;
    }

    void ljust(int newLength, contentType filling)
    {
        for (int i = 0; i < newLength - _length; i++)
        {
            node<contentType> *newNode = new node<contentType>;
            newNode->value = filling;
            newNode->next = NULL;
            last->next = newNode;
            last = newNode;
        }
        _length += newLength - _length;
    }

    void rjust(int newLength, contentType filling)
    {
        for (int i = 0; i < newLength - _length; i++)
        {
            node<contentType> *newNode = new node<contentType>;
            newNode->value = filling;
            newNode->next = first;
            first = newNode;
        }
        _length += newLength - _length;
    }

    void extend(list<contentType> *pList)
    {
        for (int i = 0; i < pList->length(); i++)
        {
            append((*pList)[i]);
        }
    }

    void clear()
    {
        first = NULL;
        last = NULL;
        _length = 0;
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
        if (index == -1)
            current = last;
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