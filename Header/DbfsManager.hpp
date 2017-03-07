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
#include <cstdint>
#include <stdexcept>
namespace shiku
{
	struct DiskLoc // sizeof(DiskLoc) == 8
	{
		using byte = char;
		uint32_t file;
		uint32_t offset;
	};
	struct Metadata // sizeof(Metadata) == 152
	{
		using byte = char;
		const static size_t NAME_LENGTH = 128;
		char Name[NAME_LENGTH];
		uint64_t Count;
		uint64_t StorageSize;
		DiskLoc FirstRecord;
	};
	struct Record // sizeof(Record) == 32
	{
		DiskLoc Next, Prev; // Cast to `Record`
		uint64_t LengthWoHeader; //Wo == Without
		// void *Data; // No need to declare a pointer
	};
	class DbfsManager
	{
	private:
		// metadata's offset of fd/mmap array
		const static int META_OFFSET = -1;
		const static size_t NAME_LENGTH = 128;
		const static size_t FILE_PER_DB = 32768;
		const static size_t BASE_SIZE = 16777216; // 16MiB
		char DBName[NAME_LENGTH];
		char RootPath[NAME_LENGTH];
		char TmpBuff[NAME_LENGTH * 8];
		int *fd, _fd[FILE_PER_DB];
		void **mem, *_mem[FILE_PER_DB];
		int *DataFileCount;
		friend void *GetAddrDlPoints(const DbfsManager &dbfsmgr, const DiskLoc &dl);
	public:
		using byte = char;
		DbfsManager(void)
		{
			throw std::runtime_error("`DbfsManager` requires all arguments");
		}
		DbfsManager(const char *_DBName, const char *_RootPath);
		~DbfsManager(void);
	};
	inline void *GetAddrDlPoints(const DbfsManager &dbfsmgr, const DiskLoc &dl)
	{
		using byte = char;
		return (byte*)dbfsmgr.mem[dl.file] + dl.offset;
	}
}
#endif // SHIKU_DBFS_MANAGER_HPP_