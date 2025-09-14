#ifndef DISK_MANAGEMENT_H
#define DISK_MANAGEMENT_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include "Page.h"


using namespace std;

Page take_page_from_disk(off_t* address, int num_frames, string file_name);
Page write_page_to_disk(off_t* address, Page* page, string file_name);

#endif