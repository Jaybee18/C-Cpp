#include <cstddef>
#include <iostream>

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

    contentType getItemAt(int index){
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

    void remove(contentType value)
    {
        node<contentType> *current = first;
        while(current->next->value != value && current->next != NULL){
            current = current->next;
        }
        if(current->next == NULL){
            return;
        }
        _length--;
    }

    /*contentType pop(int index)
    {
        if (index == 0)
        {
            node<contentType> *temp = first;
            first = first->next;
            return temp->value;
        }
        node<contentType> *current = first;
        for (int i = 0; i < index; i++)
        {
            
        }
    }*/


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

    void print(){
        std::cout << "[ ";
        for(int i = 0; i < _length; i++){
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