#include <iostream>
#include <cstring>
#include <cmath>
#include <pthread.h>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <condition_variable>

#include "Pyhsical_Memory.h"
#include "Virtual_Memory.h"
#include "Page_Table.h"
#include "Page.h"
#include "paramaters.h"



ostream& operator<<(ostream& os, const Parameters& params) {
    os << "===========================================\n"
       << "Frame size: " << params.frame_size << "\n"
       << "Number of physical pages: " << params.num_physical_pages << "\n"
       << "Number of virtual pages: " << params.num_virtual_pages << "\n"
       << "Page replacement algorithm: " << params.PageReplacementAlgorithm << "\n"
       << "Page table print: " << params.Page_table_print << "\n"
       << "Disk file name: " << params.disk_file_name << endl;
    return os;
}

ostream &operator<<(ostream &os, const Statics &statics) {
    os << "Disk reads: " << statics.disk_reads << endl;
    os << "Disk writes: " << statics.disk_writes << endl;
    os << "Page replacements: " << statics.page_replacements << endl;
    os << "Page faults: " << statics.page_faults + statics.page_replacements << endl;
    os << "Number of reads: " << statics.num_reads << endl;
    os << "Number of writes: " << statics.num_writes << endl;
    return os;
}

unsigned int diskaccesses = 0;

using namespace std;

atomic<bool> Running_1(true);
atomic<bool> Running_2(true);
atomic<bool> Running_3(true);
atomic<bool> Running_4(true);

Parameters params; 
mutex memory_mutex;
mutex fill_mutex;
mutex print_mutex;

PhysicalMemory memory;

atomic<unsigned int> combined_diskaccesses{0}; // Shared atomic counter for disk accesses
mutex cv_mutex;
condition_variable cv;
bool thread1_turn = true; // Control which thread prints first

typedef struct common_args {
    PhysicalMemory* memory;
    Statics** statics;
    int threadNumber;

}common_args;

// void* clean_R_bits(void* arg) {
//     PhysicalMemory* memory = static_cast<PhysicalMemory*>(arg);
//     while (Running_1 || Running_2) {
//         this_thread::sleep_for(chrono::milliseconds(100)); 
//         memory->clean_R_bit();
//     }
//     return NULL;
// }


void* print_page_table(void* arg) {
    VirtualMemory* virtual_memory = static_cast<VirtualMemory*>(arg);
    int thread_number = virtual_memory->get_thread_number();

    unsigned int previous_diskaccesses = 0;

    while (Running_3 && Running_4) {  // Keep running while both flags are true
        unsigned int current_diskaccesses = virtual_memory->getPageTable()->getStatics()->num_reads +
                                            virtual_memory->getPageTable()->getStatics()->num_writes;

        if (current_diskaccesses - previous_diskaccesses >= params.Page_table_print) {
            std::unique_lock<std::mutex> lock(cv_mutex);
            cv.wait(lock, [thread_number]() {
                // Wait until it's this thread's turn or until the running flags change
                return ((thread_number == 1 && thread1_turn) || (thread_number == 2 && !thread1_turn)) || 
                        !(Running_3 && Running_4); // Allow exit if either running flag is false
            });

            if (!(Running_3 && Running_4)) break; 

            virtual_memory->print();

            previous_diskaccesses = current_diskaccesses;

            thread1_turn = !thread1_turn;

            cv.notify_all();
        }
    }

    cv.notify_all();

    return NULL;
}




void* Virtual_Thread(void* arg) {
    common_args* args = static_cast<common_args*>(arg);
    Statics** statics = args->statics;
    int threadNumber = args->threadNumber;

    VirtualMemory virtual_memory(&memory, threadNumber ,params.num_virtual_pages, params.frame_size,
    params.Page_table_print, &memory_mutex);
    
    pthread_t print_thread;
    pthread_create(&print_thread, NULL, print_page_table, &virtual_memory);

    

    virtual_memory.fill(threadNumber * 1000);

    virtual_memory.merge_sort();

    
    // for (int i = 0; i < params.num_virtual_pages * params.frame_size; i++) {
    //     cout << "Thread " << threadNumber << " Virtual get = " << virtual_memory.get(threadNumber,i) << endl;
    // }

    Running_3 = false;
    Running_4 = false;

    *statics = ((*virtual_memory.getPageTable()).getStatics());

    pthread_join(print_thread, NULL);

    cout << "Thread " << threadNumber << " finished" << endl;

    int binaray_search_number;

    for (int i = 0; i < 5; i++) {
        cout << "Please enter a number to search: ";
        cin >> binaray_search_number;
        if (virtual_memory.binary_search(binaray_search_number) == -1) {
            cout << "Thread " << threadNumber << " Binary search result: " << "Not found" << endl;
        }
        else cout << "Thread " << threadNumber << " Binary search result: " << virtual_memory.binary_search(binaray_search_number) << endl;
    }

    return NULL;
}

void fill_params(Parameters* params, int argc, char const **argv) {
    if (argc != 7) {
        cout << "You must enter 6 parameters" << endl;
        exit(1);
    }
    for(int i = 1; i < argc; i++) {
        switch (i) {
            case 1:
                params->frame_size = atoi(argv[i]);
                break;
            case 2:
                params->num_physical_pages = atoi(argv[i]);
                break;
            case 3:
                params->num_virtual_pages = atoi(argv[i]);
                break;
            case 4:
                if (strcmp(argv[i], "CL") == 0 || strcmp(argv[i], "LRU") == 0) {
                    strcpy(params->PageReplacementAlgorithm, argv[i]);
                } else {
                    cout << "Invalid page replacement algorithm specified" << endl;
                    exit(1);
                }
                break;
            case 5:
                params->Page_table_print = atoi(argv[i]);
                break;
            case 6:
                strcpy(params->disk_file_name, argv[i]);
                break;
        }
    }
    params->frame_size = pow(2, params->frame_size);
    params->num_physical_pages = pow(2, params->num_physical_pages);
    params->num_virtual_pages = pow(2, params->num_virtual_pages);
}



int main(int argc, char const **argv) {
    auto begin = chrono::steady_clock::now();
    fill_params(&params, argc, argv);
    cout << params << endl;


    memory.setPageCapacity(params.num_physical_pages);
    memory.setPageSize(params.frame_size);
    memory.set_file_name(params.disk_file_name);
    memory.set_algorithm(params.PageReplacementAlgorithm);


    Statics* statics1 = new Statics;
    Statics* statics2 = new Statics;

    pthread_t thread_1, thread_2;
    common_args* args1 = new common_args{&memory, &statics1 ,1};
    pthread_create(&thread_1, NULL, Virtual_Thread, args1);

    common_args* args2 = new common_args{&memory, &statics2, 2};
    pthread_create(&thread_2, NULL, Virtual_Thread, args2);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    Running_1 = false;
    Running_2 = false;

    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - begin);

    cout << "=============================================" << endl;
    cout << "===============Print Istatics================" << endl;
    cout << "=============================================" << endl;
    cout << "===================Thread_1==================" << endl;
    cout << *statics1 << endl;
    cout << "===================Thread_2==================" << endl;
    cout << *statics2 << endl;

    cout << "Elapsed time: " << elapsed.count() << " milliseconds" << endl;

    return 0;
}





