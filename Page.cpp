#include "Page.h"

Page::Page()
{
    capacity = 0;
    size = 0;
    data = new int[capacity];
}

Page::Page(int capacity)
{
    // cout << "Page constructor" << endl;
    this->capacity = capacity;
    size = 0;
    data = new int[capacity];
}

Page::Page(const Page& other)
{
    // cout << "Page copy constructor" << endl;

    capacity = other.capacity;
    size = other.size;
    data = new int[capacity];
    for (int i = 0; i < capacity; i++)
    {
        data[i] = other.data[i];
    }
}

Page::Page(Page&& other)
{
    // cout << "Page move constructor" << endl;

    capacity = other.capacity;
    size = other.size;
    data = other.data;
    other.data = nullptr;
}


Page::~Page()
{
    delete[] data;
}

int Page::getSize() const
{
    return size;
}

void Page::setSize(int size)
{
    this->size = size;
}

int Page::getCapacity() const
{
    return capacity;
}
void Page::setCapacity(int capacity)
{
    this->capacity = capacity;
}

int Page::operator[](int index) const
{
    return data[index];
}

int& Page::operator[](int index)
{   
    return data[index];
}

void Page::operator=(const Page& other)
{
    // cout << "Page copy assignment operator" << endl;

    capacity = other.capacity;
    size = other.size;
    data = new int[capacity];

    for (int i = 0; i < capacity; i++)
    {
        data[i] = other.data[i];
        // cout << "data[" << i << "] = " << data[i] << endl;
    }


}

void Page::operator=(Page&& other)
{
    //cout << "Page move assignment operator" << endl;

    capacity = other.capacity;
    size = other.size;
    data = other.data;
    other.data = nullptr;

}



bool Page::is_full() const
{
    return size == capacity;
}

bool Page::is_empty() const
{
    return size == 0;
}