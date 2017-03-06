#include "Mmap_Win.h"
void* mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
	void *t;
	size_t len;
	HANDLE handle;
	struct stat st;
	uint64_t os = offset;
	uint32_t low = os & 0xFFFFFFFF;
	uint32_t high = (os >> 32) & 0xFFFFFFFF;
	if(!fstat(fd, &st))
		len = (size_t) st.st_size;
	else
	{
		fprintf(stderr, "mmap: Fail to determine filesize\n");
		exit(1);
	}
	if(length + offset > len)
		length = len - offset;
	if(!(flags & MAP_PRIVATE))
	{
		fprintf(stderr, "mmap: Invalid usage of mmap\n");
		exit(1);
	}
	handle = CreateFileMapping((HANDLE)_get_osfhandle(fd), 0, PAGE_WRITECOPY, 0, 0, 0);
	if(!handle)
		return MAP_FAILED;
	t = MapViewOfFileEx(handle, FILE_MAP_COPY, high, low, length, start);
	if(!CloseHandle(handle))
		fprintf(stderr, "mmap: Fail to close file mapping handle\n");
	return t ? t : MAP_FAILED;
}
int munmap(void *start, size_t length)
{
	return !UnmapViewOfFile(start);
}