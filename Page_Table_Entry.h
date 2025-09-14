#ifndef PAGE_TABLE_ENTRY_H
#define PAGE_TABLE_ENTRY_H

#include <iostream>
#include <chrono>

#include "paramaters.h"

class PageTable;



using namespace std;

class PageTableEntry
{
    public:

        
        PageTableEntry();
        PageTableEntry(PageTable* page_table);
        ~PageTableEntry();

        off_t getAddress() const;
        void setAddress(off_t address);

        int getThreadNumber() const;
        void setThreadNumber(int thread_number);

        friend ostream& operator<<(ostream& os, const PageTableEntry& entry);

        void setLastAccessed(chrono::steady_clock::time_point last_accessed);
        chrono::steady_clock::time_point getLastAccessed() const;

        chrono::milliseconds duration(chrono::steady_clock::time_point current_time) const;
        

        bool isReferenced() const;
        void setReferenced(bool referenced);

        bool isModified() const;
        void setModified(bool modified);

        PageTable* getPageTable() ;

        void setPageTable(PageTable* page_table);

        


    private:
        off_t address;
        chrono::steady_clock::time_point last_accessed;
        int thread_number;
        bool referenced;
        bool modified;
        
        PageTable* page_table;
       
};

#endif // PAGE_TABLE_ENTRY_H