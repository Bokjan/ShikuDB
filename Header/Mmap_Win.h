#ifndef SHIKU_MMAP_WIN_HPP_
#define SHIKU_MMAP_WIN_HPP_
#include <io.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <sys/stat.h>
#define PROT_READ  1
#define PROT_WRITE 2
#define MAP_FAILED ((void*) - 1)
#define MAP_SHARED	0x01
#define MAP_PRIVATE	0x02
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *start, size_t length);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SHIKU_MMAP_WIN_HPP_