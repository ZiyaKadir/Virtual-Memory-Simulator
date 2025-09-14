#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <iostream>

using namespace std;


typedef struct Parameters{

    unsigned int frame_size;
    unsigned int num_physical_pages;
    unsigned int num_virtual_pages;
    char PageReplacementAlgorithm[4];
    unsigned int Page_table_print;
    char disk_file_name[100];

} Parameters;

typedef struct Statics{
    int disk_reads = 0;
    int disk_writes = 0; 
    int page_replacements = 0;
    int page_faults = 0;
    int num_reads = 0;
    int num_writes = 0;


    // Overload the assignment operator
    Statics& operator=(const Statics& other) {
        if (this != &other) {  // Protect against invalid self-assignment
            disk_reads = other.disk_reads;
            disk_writes = other.disk_writes;
            page_replacements = other.page_replacements;
            page_faults = other.page_faults;
            num_reads = other.num_reads;
            num_writes = other.num_writes;
        }
        // Return a reference to this instance
        return *this;
    }

}Statics;






#endif




