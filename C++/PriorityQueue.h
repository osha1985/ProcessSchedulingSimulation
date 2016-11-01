#ifndef PRIORITYQUEUE_H_INCLUDED
#define PRIORITYQUEUE_H_INCLUDED
#include <vector>
#include <algorithm>
template <class T>
class PriorityQueue
{
private:
    std::vector <T> storage;
    int breadthFirstSearch(T);
public:
    void push_back(T);
    void remove(T);
    void printPriorityQueue();
    void peek(T &);
    T& top ();
    T& operator[](int);
    void pop();
    int size();
    bool empty();
};

template <class T>
int PriorityQueue<T>::breadthFirstSearch(T value)
{
    int index;
    for(index = 0; index < storage.size(); index++)
    {
        if(storage[index] == value)
        {
            return index;
        }
    }
}

template <class T>
void PriorityQueue<T>::push_back(T value)
{
    storage.push_back(value);
    for(int i = storage.size() - 1 ; i > 0 ; i = ( i - 1 ) / 2 )
    {
        if(storage[i] < storage[( i - 1 ) / 2 ] )
        {
            std::swap(storage[ i ], storage[ ( i - 1 ) / 2 ] );
        }
        else
        {
            break;
        }
    }
}

template <class T>
void PriorityQueue<T>::remove(T value)
{
    int index;
    int nextNode;
    index = breadthFirstSearch(value);
    storage[index] = storage.back();
    storage.pop_back();
    int i = index;
    while(2 * i + 1 < storage.length())
    {
        nextNode = 2 * i + 1;
        if(2 * i + 2 < storage.length() && storage[2 * i + 2] < storage[ 2 * i + 1 ])
        {
            nextNode = 2 * i + 2;
        }
        if(storage[i] <= storage[nextNode])
        {
            break;
        }
        std::swap(storage[i],storage[nextNode]);
        i = nextNode;
    }
}

template <class T>
void PriorityQueue<T>::pop()
{
    int nextNode;
    storage[0] = storage.back();
    storage.pop_back();
    int i = 0;
    while(2 * i + 1 < storage.size())
    {
        nextNode = 2 * i + 1;
        if(2 * i + 2 < storage.size() && storage[2 * i + 2] < storage[ 2 * i + 1 ])
        {
            nextNode = 2 * i + 2;
        }
        if(storage[i] <= storage[nextNode])
        {
            break;
        }
        std::swap(storage[i],storage[nextNode]);
        i = nextNode;
    }
}

template <class T>
void PriorityQueue<T>::printPriorityQueue()
{
    for(int i = 0; i < storage.size(); i++)
    {
        std::cout<<storage[i]<<" ";
    }
    std::cout<<"\n";
}

template <class T>
void PriorityQueue<T>::peek(T& value)
{
    if(storage.size() == 0)
    {
        std::cout<<"The priority queue is empty"<<"\n";
    }
    else
    {
        value = storage[0];
    }
}

template <class T>
T& PriorityQueue<T>::top()
{
    return storage[0];
}

template <class T>
T& PriorityQueue<T>::operator[](int index)
{
    return storage[index];
}

template <class T>
int PriorityQueue<T>::size()
{
    return storage.size();
}

template <class T>
bool PriorityQueue<T>::empty()
{
    return storage.empty();
}
#endif // PRIORITYQUEUE_H_INCLUDED
