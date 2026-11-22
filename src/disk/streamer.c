#include "streamer.h"
#include "memory/heap/kheap.h"

struct disk_stream* diskstreamer_new(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstreamer_seek(struct disk_stream* stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total)
{
    // pos = 513이라 가정 시 sector는 1, offset은 1
    int sector = stream->pos / 	PEACHOS_SECTOR_SIZE;
    int offset = stream->pos % PEACHOS_SECTOR_SIZE;
    char buf[PEACHOS_SECTOR_SIZE];

    // buf에 1 sector 담아옴 
    int res = disk_read_block(stream->disk, sector, 1, buf);
    if (res < 0)
    {
        goto out;
    }

    // 1 sector보다 많은 값 요청 시 1 sector 먼저 읽게함 
    int total_to_read = total > PEACHOS_SECTOR_SIZE ? PEACHOS_SECTOR_SIZE : total;
    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset+i];
	// *(char*) out = buf[offset+i]
	// out++
    }

    // 마지막으로 읽은 값 업데이트 
    stream->pos += total_to_read;
    if (total > PEACHOS_SECTOR_SIZE)
    {
	// 재귀 호출로 남은 데이터 읽기 
        res = diskstreamer_read(stream, out, total-PEACHOS_SECTOR_SIZE);
    }

out:
    return res;    

}

void diskstreamer_close(struct disk_stream* stream)
{
    kfree(stream);
}
