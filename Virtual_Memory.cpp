#include "Virtual_Memory.h"
#include "Page_Table.h"

VirtualMemory::VirtualMemory(PhysicalMemory* pyhsical_memory, int Thread_number,
unsigned int num_pages, unsigned int page_size, unsigned int page_print , mutex* memory_mutex)
{   
    this->num_pages = num_pages;
    this->page_size = page_size;

    this->page_table = new PageTable(pyhsical_memory,Thread_number, num_pages,page_size);
    
    this->page_table->setPhysicalMemory(pyhsical_memory);
    this->page_table->setCapacity(num_pages);
    this->page_table->setCount(0);

    this->memory_mutex = memory_mutex;

    this->page_print = page_print;

    this->Thread_number = Thread_number;

};

VirtualMemory::~VirtualMemory()
{
};



PageTable* VirtualMemory::getPageTable() const
{
    return page_table;
}

void VirtualMemory::setPageTable(PageTable page_table)
{
    *this->page_table = page_table;
}






void VirtualMemory::fill(int seed){

    

    int value = 0;
    for (int i = 0; i < num_pages * page_size; i++){
        {
            lock_guard<mutex> lock(*memory_mutex);
            value = rand() %1000;
            // cout << "value = " << value << endl;
        }
        set(Thread_number,i,value);
    }
}


void VirtualMemory::set(unsigned int Thread_number,unsigned int index, int value)
{   
    lock_guard<mutex> lock(*memory_mutex);

    int page_index = index / page_size; // page index
    int offset = index % page_size; // number index in page

    // cout << "page_index = " << page_index << " offset = " << offset << " value = " << value << endl;




    page_table->set(page_index, offset, value);

}

int VirtualMemory::get(unsigned int Thread_number, unsigned int index)
{
    lock_guard<mutex> lock(*memory_mutex);

    int page_index = index / page_size; // page index
    int offset = index % page_size; // number index in page
    
    // cout << "get page_index = " << page_index << " offset = " << offset << " value = " << page_table->get(page_index, offset) << endl;


    return page_table->get(page_index, offset);
}


void VirtualMemory::clean_R_bit()
{   
    lock_guard<mutex> lock(*memory_mutex);

    page_table->clean_R_bit();
}

void VirtualMemory::print() const
{
    cout << "=========================" <<  "Virtual Memory " << Thread_number << "'s Page Table=========================" << endl;
    
    cout << *page_table << endl;
}


void VirtualMemory::merge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    VirtualMemory leftMemory(this->page_table->getPhysicalMemory(),Thread_number ,n1, 1, page_print,this->memory_mutex);
    VirtualMemory rightMemory(this->page_table->getPhysicalMemory(), Thread_number,n2, 2, page_print,this->memory_mutex);

    for (int i = 0; i < n1; i++) {
        leftMemory.set(Thread_number,i, get(Thread_number,left + i));
    }
    for (int j = 0; j < n2; j++) {
        rightMemory.set(Thread_number,j, get(Thread_number,mid + 1 + j));
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftMemory.get(Thread_number,i) <= rightMemory.get(Thread_number,j)) {
            set(Thread_number,k, leftMemory.get(Thread_number,i));
            i++;
        } else {
            set(Thread_number,k, rightMemory.get(Thread_number,j));
            j++;
        }
        k++;
    }

    while (i < n1) {
        set(Thread_number,k,leftMemory.get(Thread_number,i));
        i++;
        k++;
    }

    while (j < n2) {
        set(Thread_number,k,rightMemory.get(Thread_number,j));
        j++;
        k++;
    }

    getPageTable()->getStatics()->page_faults += leftMemory.getPageTable()->getStatics2().page_faults + rightMemory.getPageTable()->getStatics2().page_faults;
    getPageTable()->getStatics()->page_replacements += leftMemory.getPageTable()->getStatics2().page_replacements + rightMemory.getPageTable()->getStatics2().page_replacements;
    getPageTable()->getStatics()->disk_reads += leftMemory.getPageTable()->getStatics2().disk_reads + rightMemory.getPageTable()->getStatics2().disk_reads;
    getPageTable()->getStatics()->disk_writes += leftMemory.getPageTable()->getStatics2().disk_writes + rightMemory.getPageTable()->getStatics2().disk_writes;
    getPageTable()->getStatics()->num_reads += leftMemory.getPageTable()->getStatics2().num_reads + rightMemory.getPageTable()->getStatics2().num_reads;
    getPageTable()->getStatics()->num_writes += leftMemory.getPageTable()->getStatics2().num_writes + rightMemory.getPageTable()->getStatics2().num_writes;
   

}

void VirtualMemory::mergeSort(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(left, mid);
        mergeSort(mid + 1, right);

        merge(left, mid, right);
    }
}

void VirtualMemory::merge_sort() {
    mergeSort(0, num_pages * page_size - 1);
}


int VirtualMemory::binary_search(int value)
{
    int left = 0;
    int right = num_pages * page_size - 1;
    int mid;

    while (left <= right) {
        mid = left + (right - left) / 2;

        if (get(Thread_number,mid) == value) {
            return mid;
        } else if (get(Thread_number,mid) < value) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}