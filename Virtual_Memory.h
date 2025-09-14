#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <iostream>
#include <mutex>
#include "Page_Table.h"
#include "Pyhsical_Memory.h"
#include "cstdlib"
#include "paramaters.h"

using namespace std;

class VirtualMemory
{
    public:

        VirtualMemory(PhysicalMemory* pyhsical_memory ,int Thread_number,unsigned int num_pages, unsigned int page_size,unsigned int page_print, mutex* memory_mutex);
        ~VirtualMemory();


        PageTable* getPageTable() const;
        void setPageTable(PageTable page_table);



        
        void mergeSort(int left, int right);
        void merge(int left, int middle, int right);
        void merge_sort();

        int get_thread_number() const{
            return Thread_number;
        }
        


        int binary_search(int value);

    
        void fill(int seed);

        void set(unsigned int Thread_number, unsigned int index, int value);
        int get(unsigned int Thread_number, unsigned int index);
   
        

        void clean_R_bit();
        
        void print() const;
    
    private:
        PageTable* page_table;
        unsigned int num_pages;
        unsigned int page_size;


        unsigned int page_print;

        unsigned int Thread_number;

        mutex* memory_mutex; // to prevent collision

        
};




#endif 