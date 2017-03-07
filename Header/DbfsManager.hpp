#ifndef SHIKU_DBFS_MANAGER_HPP_
#define SHIKU_DBFS_MANAGER_HPP_
#ifdef _WIN32 // Windows
#include "mman.h"
#else // *nix
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#endif // _WIN32
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
namespace shiku
{
	class DbfsManager
	{
	private:
		// metadata's offset of fd/mmap array
		const static int META_OFFSET = -1;
		const static int NAME_LENGTH = 128;
		const static int FILE_PER_DB = 32768;
		const static size_t BASE_SIZE = 16777216; // 16MiB
		char DBName[NAME_LENGTH];
		char RootPath[NAME_LENGTH];
		char TmpBuff[NAME_LENGTH * 8];
		int *fd, _fd[FILE_PER_DB];
		void **mem, *_mem[FILE_PER_DB];
		int *DataFileCount;
	public:
		using byte = char;
		DbfsManager(void)
		{
			throw std::runtime_error("`DbfsManager` requires all arguments");
		}
		DbfsManager(const char *_DBName, const char *_RootPath);
		~DbfsManager(void);
	};
}
#endif // SHIKU_DBFS_MANAGER_HPP_