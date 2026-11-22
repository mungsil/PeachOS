#include "memory.h"

void* memset(void* ptr, int c, size_t size)
{
    char* c_ptr = (char*) ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char) c;
    }
    return ptr;
}

// s1과 s2가 가리키는 메모리 영역의 내용을 count byte 만큼 비교 
// 처음으로 다른 바이트를 발견했을 때, s1이 가리키는 값이 더 작으면 -1, 더 크면 1 반환, 동일하면 0 반환
int memcmp(void* s1, void* s2, int count)
{
    char* c1 = s1;
    char* c2 = s2;
    while(count-- > 0)
    {
        if (*c1++ != *c2++)
	{
	    return c1[-1] < c2[-1] ? -1 : 1;
	}
    }
    return 0;
}
