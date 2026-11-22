#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "kernel.h"

struct filesystem* filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem()
{
    int i = 0;
    for (i=0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0) // free라는 뜻 
	{
	    return &filesystems[i];
	}	
    }

    // free 없음 -> 0 반환 
    return 0;
}

void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs = fs_get_free_filesystem();
    if (!fs)
    {
        print("Problem inserting filesystem");
        while(1) {}	
    }

    *fs = filesystem;
}

// static fs means that are built into the core kernel.
static void fs_static_load()
{
    // fs_insert_filesystem(fat16_init());
}

// this function will load all the available file systems that we are aware of
void fs_load()
{
    memset(filesystems, 0, size(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;

    // loop 돌며 사용 가능한(free) fd 찾기
    for (int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; i++)
    {
        if (file_descriptors[i] == 0)
	{
	    // fd를 위해 some memory 할당 
	    struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
	    // Descriptors start at 1
	    desc->index = i + 1;
	    file_descriptors[i] = desc;
	    *desc_out = desc;
	    // everything went okay
	    res = 0;
	    break;
	}
    }

    return res;
}

static struct file_descriptor* file_get_descriptor(int fd)
{
    if (fd <= 0 || fd >= PEACHOS_MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }

    // Descriptors start at 1
    int index = fd - 1;
    return file_descriptors[index];

}

struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs = 0;
    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
	{
	    fs = filesystems[i];
	    break;
	}
    }

    return fs;
}

int fopen(const char* filename, const char* mode)
{
    // IO error
    return -EIO;
}
