#include "Disk_management.h"

using namespace std;


Page take_page_from_disk(off_t* address, int page_size, string diskFileName) {

    int fd = open(diskFileName.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file for reading." << std::endl;
        exit(1); 
    }

    Page page(page_size);

    if (lseek(fd, *address, SEEK_SET) == -1) {
        std::cerr << "Failed to seek to the specified address." << std::endl;
        close(fd);
        exit(1);
    }

    for (int i = 0; i < page_size; i++) {
        if (read(fd, &page[i], sizeof(int)) != sizeof(int)) {
            std::cerr << "Failed to read complete data for page[" << i << "]." << std::endl;
            close(fd);
            exit(1);
        }
    }

    
    close(fd);

    return page;
}




Page write_page_to_disk(off_t* address, Page* page, string diskFileName) {

    int fd = open(diskFileName.c_str(), O_WRONLY | O_CREAT, 0777);
    if (fd == -1) {
        cerr << "Failed to open file for writing." << endl;
        exit(1);
    }

    if (*address == -1) {
        *address = lseek(fd, 0, SEEK_END);
    } else {
        lseek(fd, *address, SEEK_SET);
    }
    
    for (int i = 0; i < page->getCapacity(); i++) {
        write(fd, &page->operator[](i), sizeof(int));
    }

    close(fd);


    return *page;
}


