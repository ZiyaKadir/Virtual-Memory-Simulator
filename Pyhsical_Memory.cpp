#include "Pyhsical_Memory.h"
#include "Disk_management.h"
#include <math.h>

#include "Page_Table.h"





PhysicalMemory::PhysicalMemory()
{
    page_capacity = 0;
    page_size = 0;
    page_count = 0;
   
}

PhysicalMemory:: PhysicalMemory(int page_capacity, int page_size, char* disk_file_name) {
    this->page_capacity = page_capacity;
    this->page_size = page_size;
    this->page_count = 0;

    this->file_name = disk_file_name;
}

PhysicalMemory::PhysicalMemory(const PhysicalMemory& other)
{
    page_capacity = other.page_capacity;
    page_size = other.page_size;
    page_count = other.page_count;
    memory = other.memory;
    reference_table = other.reference_table;
}

PhysicalMemory& PhysicalMemory::operator=(const PhysicalMemory& other)
{
    page_capacity = other.page_capacity;
    page_size = other.page_size;
    page_count = other.page_count;
    memory = other.memory;
    reference_table = other.reference_table;
    return *this;
}


PhysicalMemory::PhysicalMemory(PhysicalMemory&& other)
{
    page_capacity = other.page_capacity;
    page_size = other.page_size;
    page_count = other.page_count;
    memory = other.memory;
    reference_table = other.reference_table;
}


PhysicalMemory::~PhysicalMemory()
{
}

bool PhysicalMemory::is_full() const
{
    return page_capacity == page_count;
}


void PhysicalMemory::setPageSize(int page_size)
{
    this->page_size = page_size;
}

void PhysicalMemory::setPageCapacity(int page_capacity)
{
    this->page_capacity = page_capacity;
}

void PhysicalMemory::setPageCount(int page_count)
{
    this->page_count = page_count;
}


int PhysicalMemory::getPageCount() const
{
    return page_count;
}

int PhysicalMemory::getPageSize() const
{
    return page_size;
}

int PhysicalMemory::getPageCapacity() const
{
    return page_capacity;
}


void PhysicalMemory::set_file_name(char* file_name)
{
    this->file_name = file_name;
}

void PhysicalMemory::set_algorithm(char* algorithm)
{
    this->algorithm = algorithm;
}

PageTableEntry* PhysicalMemory::get(off_t address)
{
    return reference_table[address];
}


void PhysicalMemory::set(off_t address, PageTableEntry* page_table_entry)
{
    reference_table[address] = page_table_entry;
}


off_t PhysicalMemory::LRU_page_remove()
{
    
    chrono::milliseconds max = chrono::milliseconds::min();
    chrono::steady_clock::time_point current_time = chrono::steady_clock::now();
    off_t address;

    for (auto& entry : reference_table)
    {
        chrono::milliseconds duration = entry.second->duration(current_time);
        if (duration > max)
        {
            max = duration;
            address = entry.first;
        }
    }

    if (reference_table[address]->isModified())
    {
        write_page_to_disk(&address, &memory[address], file_name);
        reference_table[address]->getPageTable()->getStatics()->disk_writes++;
        reference_table[address]->getPageTable()->getStatics()->page_replacements++;
    }   
    else{
        reference_table[address]->getPageTable()->getStatics()->page_replacements++;
    }

    reference_table[address]->setModified(false);
    reference_table[address]->setReferenced(false);
   
    return address;
}

off_t PhysicalMemory::CL_page_remove()
{   

    for (auto& entry : reference_table)
    {
        if (entry.second->isReferenced() == false)
        {
            off_t address = entry.first;

            if (entry.second->isModified())
            {
                write_page_to_disk(&address, &memory[address], file_name);

                reference_table[address]->getPageTable()->getStatics()->disk_writes++;
                reference_table[address]->getPageTable()->getStatics()->page_replacements++;
                
            }
            else{
                reference_table[address]->getPageTable()->getStatics()->page_replacements++;
                
            }
            
            reference_table[address]->setModified(false);
            reference_table[address]->setReferenced(false);
            return address;
        }
        else{
            entry.second->setReferenced(false);
        }
    }


    // if didn't find any page that is not referenced
    for (auto& entry : reference_table)
    {
        return entry.first;
    }
    return -1; // error handling
}



Page& PhysicalMemory::operator[](off_t* address)
{       

    if((*address) == -1){
        // the page is not created yet
        
        if(is_full() != true){
            //cout << "address is" << *address << endl;           
            Page page(page_size);
            memory[*address] = write_page_to_disk(address , &page, file_name);    
          
            page_count++;
            //cout << "address after take page from disk is = " << (*address) << endl;
            return memory[*address]; 
        }
        else{
            //cout << "LRU working" << endl;
            off_t erase_address;
            if (algorithm == "LRU")  erase_address = LRU_page_remove();
            else if (algorithm == "CL")  erase_address = CL_page_remove();
            else{
                cout << "Algorithm is not defined" << endl;
                exit(1);
            }
            memory.erase(erase_address);
            reference_table.erase(erase_address);
            
            //reference i unutmusum
            // reference_table.erase(erase_address);


            Page page(page_size);
            memory[*address] = write_page_to_disk(address , &page, file_name);
            

            return memory[*address];
        }
    }
    else{ // page is created that's means that they have actual address
        if(memory.find(*address) == memory.end()){ // if *address is not founded
        /* 
            cout << "page not found" << endl;
            cout << "take from the disk" << endl;
        */
            if(is_full()){
                off_t erase_address;
                if (algorithm == "LRU")  erase_address = LRU_page_remove();
                else if (algorithm == "CL")  erase_address = CL_page_remove();
                else{
                    cout << "Algorithm is not defined" << endl;
                    exit(1);
                }
                
                memory.erase(erase_address);
                reference_table.erase(erase_address);
                Page page = take_page_from_disk(address, page_size, file_name);
                
             
                memory[*address] = page;
                return memory[*address];
            } else {
                Page page = take_page_from_disk(address, page_size, file_name);
              
                memory[*address] = page;
                page_count++;
                return memory[*address];
            }
        } else {
            return memory[*address];
        }  
    }

}

void PhysicalMemory::clean_R_bit()
{
    for (auto& entry : reference_table)
    {
        entry.second->setReferenced(false);
    }
}



