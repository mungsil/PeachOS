#include "disk.h"
#include "io/io.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"

// disk 구조체의 변수 disk
struct disk disk;

// outb (port, value) - port: value를 쓸 port
// insw(port) - port로부터 2byte 읽음 
int disk_read_sector(int lba, int total, void* buf)
{
    // 0xE0 = drive 제어 flag 값으로, LBA 모드를 사용하며 마스터 drive를 선택한다는 뜻
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Wait for the buffer to be ready
	char c = insb(0x1F7);
	while(!(c & 0x08))
	{
	    c = insb(0x1F7);
	}

	// Copy from hard disk to memory
	for (int i = 0; i < 256; i++)
	{
	    // 0x1F0: 실제 디스크 데이터가 오고가는 16bit data 포트 
	    *ptr = insw(0x1F0);
	    ptr++;
	}
    }

    return 0;
}

// 함수명과 다르게 search 하지 않음 
// search 없이 primary disk 가리킴 
void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = PEACHOS_DISK_TYPE_REAL;
    disk.sector_size = PEACHOS_SECTOR_SIZE;
    disk.filesystem = fs_resolve(&disk);
}

struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;

    return &disk;
}

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf)
{
    if (idisk != &disk)
    {
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}	
