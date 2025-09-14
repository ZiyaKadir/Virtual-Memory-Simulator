#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <iostream>
#include <unordered_map>
#include <iomanip>

#include "Pyhsical_Memory.h"
#include "paramaters.h"


using namespace std;


class PageTable
{
    public:
        PageTable(PhysicalMemory* physical_memory, unsigned int Thread_number,
         int page_entry_capacity, int page_entry_size);
        PageTable();
        PageTable(const PageTable& page_table);
        PageTable& operator=(const PageTable& page_table);
        PageTable(PageTable&& page_table);
        ~PageTable();

        PageTableEntry& operator[](int page_number);
        bool is_full() const;

        friend ostream& operator<<(ostream& os, const PageTable& page_table);

        void set (int page_number, int index, int value);
        int get (int page_number, int index);

        void setPhysicalMemory(PhysicalMemory* physical_memory);
        
        PhysicalMemory* getPhysicalMemory() const;

        Statics* getStatics(){
            return &statics;
        }
        Statics getStatics2() const{
            return statics;
        }

        void clean_R_bit();

        void setCapacity(int page_entry_count);
        void setCount(int page_entry_capacity);

        int getCapacity() const;
        int getCount() const;
        

    private:
    
        PhysicalMemory* physical_memory;
        unordered_map<int, PageTableEntry> table;
        unsigned int Thread_number;


        int page_entry_capacity;
        int page_entry_count;
        
        Statics statics;
};


#endif // PAGE_TABLE_H