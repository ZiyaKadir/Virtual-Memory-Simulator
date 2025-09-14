#ifndef PAGE_H
#define PAGE_H

#include <iostream>

using namespace std;


class Page
{
    public:
    Page();
    Page(int capacity);
    Page (const Page& other);
    Page (Page&& other);
    ~Page();

    int getSize() const;
    void setSize(int size);

    int* getData() const { return data;}

    int getCapacity() const;
    void setCapacity(int capacity);

    int operator[](int index) const;
    int& operator[](int index);

    void operator=(const Page& other);
    void operator=(Page&& other);

    bool is_full()const;

    bool is_empty()const;

    friend ostream& operator<<(ostream& os, const Page& page);


    private:
        int capacity;
        int size;
        int* data;
};





#endif // PAGE_H