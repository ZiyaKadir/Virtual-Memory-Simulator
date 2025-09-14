#include <iostream>
#include "Page_Table.h"

using namespace std;

PageTable::PageTable(PhysicalMemory *physical_memory, unsigned int Thread_number, int page_entry_capacity, int page_entry_size)
{
    // cout << "PageTable constructor" << endl;

    this->physical_memory = physical_memory;
    this->page_entry_capacity = page_entry_capacity;
    this->page_entry_count = 0;


    for (int i = 0; i < page_entry_capacity; i++)
    {

        // cout << "Buraya dikkat et" << endl;
        PageTableEntry page_table_entry = PageTableEntry(this);
        table[i] = page_table_entry;
        
    }
  
}

PageTable::PageTable()
{
    this->physical_memory = nullptr;
    this->page_entry_capacity = 0;
    this->page_entry_count = 0;
}

PageTable::PageTable(const PageTable& page_table)
{
    this->physical_memory = page_table.physical_memory;
    this->table = page_table.table;
    this->page_entry_capacity = page_table.page_entry_capacity;
    this->page_entry_count = page_table.page_entry_count;
}

PageTable::PageTable(PageTable&& page_table)
{
    this->physical_memory = page_table.physical_memory;
    this->table = page_table.table;
    this->page_entry_capacity = page_table.page_entry_capacity;
    this->page_entry_count = page_table.page_entry_count;
}

PageTable& PageTable::operator=(const PageTable& page_table)
{
    this->physical_memory = page_table.physical_memory;
    this->table = page_table.table;
    this->page_entry_capacity = page_table.page_entry_capacity;
    this->page_entry_count = page_table.page_entry_count;
    return *this;
}


void PageTable::setPhysicalMemory(PhysicalMemory* physical_memory)
{
    this->physical_memory = physical_memory;
}

PhysicalMemory* PageTable::getPhysicalMemory() const
{
    return physical_memory;
}

void PageTable::setCapacity(int page_entry_capacity)
{
    this->page_entry_capacity = page_entry_capacity;
}

void PageTable::setCount(int page_entry_count)
{
    this->page_entry_count = page_entry_count;
}

int PageTable::getCapacity() const
{
    return page_entry_capacity;
}

int PageTable::getCount() const
{
    return page_entry_count;
}


PageTable::~PageTable()
{
}

PageTableEntry& PageTable::operator[](int page_index)
{
    return table[page_index]; // take entry from table
}

bool PageTable::is_full() const
{
    return page_entry_capacity == table.size();
    // return page_entry_capacity == page_entry_count;
}


ostream& operator<<(ostream& os, const PageTable& page_table)
{
    os << "Page Table Entries:\n";
    os << "-------------------------------------------------------------\n";
    os << "| Page Number | Address   | Last Accessed            | Referenced | Modified |\n";
    os << "-------------------------------------------------------------\n";

    for (const auto& entry_pair : page_table.table)
    {
        int page_number = entry_pair.first;
        const PageTableEntry& entry = entry_pair.second;

        os << "| " << setw(12) << page_number
        << " | " << setw(9) << entry.getAddress()
        << " | " << setw(24) << chrono::duration_cast<chrono::milliseconds>(
                                        entry.getLastAccessed().time_since_epoch()).count() << " ms"
        << " | " << setw(10) << (entry.isReferenced() ? "Yes" : "No")
        << " | " << setw(9) << (entry.isModified() ? "Yes" : "No")
        << " |\n";
    }

    os << "-------------------------------------------------------------\n";

    return os;

}





void PageTable::set(int page_index, int index, int value)
{
    off_t address = table[page_index].getAddress();
    
    unsigned int previous = statics.page_replacements;

    if (address == -1)
    {
        statics.page_faults++;
        statics.disk_writes++;
    }

    table[page_index].setModified(true); // set modified true
    table[page_index].setReferenced(true); // set referenced true
    table[page_index].setLastAccessed(chrono::steady_clock::now()); // set last accessed time

    Page* page = &((*physical_memory)[&address]); // get the page from physical memory

    physical_memory->set(address, &(table[page_index])); // set pageTable entry to physical memory

    table[page_index].setAddress(address);


    statics.num_writes++;
    if (previous != statics.page_replacements)
    {
        statics.disk_writes++;

    }
    physical_memory->set(address, &table[page_index]);
    (*page)[index] = value; // be careful because you wrıte on the copy not actual adress

    
}

int PageTable::get(int page_index, int index)
{
    off_t address = table[page_index].getAddress();

    unsigned int previous = statics.page_replacements;

    if (address == -1)
    {
        statics.page_faults++;
        statics.disk_reads++;
    }

    table[page_index].setReferenced(true); // set modified true
    table[page_index].setLastAccessed(chrono::steady_clock::now()); // set last accessed time

    Page* page = &((*physical_memory)[&address]); 
    
    statics.num_reads++;
    if (previous != statics.page_replacements)
    {
        statics.disk_reads++;
    }
    physical_memory->set(address, &table[page_index]);
    return (*page)[index];
}

void PageTable::clean_R_bit()
{
    
    for (auto& entry : table)
    {
        entry.second.setReferenced(false);
    }
    
}