#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

// unsigned int 자료형을 PEACHOS_DISK_TYPE으로 재정의 
typedef unsigned int PEACHOS_DISK_TYPE;


// Represents a real physical hard disk
# define PEACHOS_DISK_TYPE_REAL 0

struct disk
{
    PEACHOS_DISK_TYPE type;
    int sector_size;

    struct filesystem* filesystem;
};

void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_sector(int lba, int total, void* buf);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif
