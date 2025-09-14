#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H


#include <iostream>
#include <unordered_map>


// #include "Page_Table.h"
#include "Page_Table_Entry.h"
#include "Page.h"
#include "paramaters.h"


class PageTable;

using namespace std;


class PhysicalMemory
{

    public:
        PhysicalMemory();
        PhysicalMemory(int page_capacity, int page_size, char* file_name);
        PhysicalMemory(const PhysicalMemory& physical_memory);
        PhysicalMemory(PhysicalMemory&& physical_memory);
        PhysicalMemory& operator=(const PhysicalMemory& physical_memory);
        
        ~PhysicalMemory();

        Page& operator[](off_t* address);
        
        bool is_full() const;
        
        void setPageSize(int page_size);
        void setPageCapacity(int page_capacity);
        void setPageCount(int page_count);

        int getPageCount() const;
        int getPageSize() const;
        int getPageCapacity() const;

        void set_file_name(char* file_name);
        void set_algorithm(char* algorithm);
        

        void set(off_t address, PageTableEntry* page_table_entry);
        PageTableEntry* get(off_t address);

        void clean_R_bit();

        off_t LRU_page_remove();
        off_t CL_page_remove();

    
    private:
        unordered_map<off_t, Page> memory;
        unordered_map<off_t, PageTableEntry*> reference_table;


        int page_size;
        int page_capacity; 
        int page_count;

        string file_name;
        string algorithm;
              
};



#endif