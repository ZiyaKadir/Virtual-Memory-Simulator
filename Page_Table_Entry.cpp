#include "Page_Table_Entry.h"

PageTableEntry::PageTableEntry()
{
    this->address = -1;
    this->referenced = false;
    this->modified = false;

    this->page_table = nullptr;
}

PageTableEntry::PageTableEntry(PageTable* page_table)
{
    this->address = -1;
    this->referenced = false;
    this->modified = false;

    this->page_table = page_table;
}


PageTableEntry::~PageTableEntry()
{
}

int PageTableEntry::getThreadNumber() const
{
    return thread_number;
}

void PageTableEntry::setThreadNumber(int thread_number)
{
    this->thread_number = thread_number;
}

off_t PageTableEntry::getAddress() const
{
    return address;
}

void PageTableEntry::setAddress(off_t address)
{
    this->address = address;
}

PageTable* PageTableEntry::getPageTable() 
{
    return page_table;
}

void PageTableEntry::setPageTable(PageTable* page_table)
{
    this->page_table = page_table;
}


bool PageTableEntry::isReferenced() const
{
    return referenced;
}

void PageTableEntry::setReferenced(bool referenced)
{
    this->referenced = referenced;
}

bool PageTableEntry::isModified() const
{
    return modified;
}

void PageTableEntry::setModified(bool modified)
{
    this->modified = modified;
}

chrono::steady_clock::time_point PageTableEntry::getLastAccessed() const
{
    return last_accessed;
}

void PageTableEntry::setLastAccessed(chrono::steady_clock::time_point last_accessed)
{
    this->last_accessed = last_accessed;
}


chrono::milliseconds PageTableEntry::duration(chrono::steady_clock::time_point current_time) const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_accessed);
}


ostream& operator<<(ostream& os, const PageTableEntry& entry)
{
    os << "Address: " << entry.address << " Referenced: " << entry.referenced << " Modified: " << entry.modified;
    return os;
}