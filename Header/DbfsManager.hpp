#ifndef SHIKU_DBFS_MANAGER_HPP_
#define SHIKU_DBFS_MANAGER_HPP_
#ifdef _WIN32 // Windows
#include "mman.h"
#else // *nix
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#endif // _WIN32
#include <map>
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <utility>
#include <cstring>
#include <cstdint>
#include <stdexcept>
using std::string;
namespace shiku
{
	struct DiskLoc // sizeof(DiskLoc) == 8
	{
		using byte = char;
		const static int32_t NullLoc = -1;
		int32_t file;
		int32_t offset;
		DiskLoc(void): 
			file(NullLoc), offset(NullLoc) { }
		void Assign(int32_t f, int32_t o)
		{
			file = f;
			offset = o;
		}
		bool IsNullLoc(void)
		{
			return file == NullLoc;
		}
		static inline DiskLoc GenNullLoc(void)
		{
			return DiskLoc();
		}
	};
	bool operator == (const DiskLoc &a, const DiskLoc &b);
	struct Metadata // sizeof(Metadata) == 160
	{
		using byte = char;
		const static size_t NAME_LENGTH = 128;
		char Name[NAME_LENGTH];
		uint64_t Count;
		uint64_t UsedSize;
		uint64_t TotalSize;
		DiskLoc FirstRecord;
	};
	struct Record // sizeof(Record) == 32
	{
		DiskLoc self, prev, next; // Cast to `Record`
		uint64_t length; // Length with header
		// void *Data; // No need to declare a pointer
		inline uint64_t LengthWoHeader(void)
		{
			return length - sizeof(Record);
		}
	};
	// First 4096 bytes of *.meta is reserved
	namespace META_LAYOUT
	{
		const static size_t META_START_AT = 4096;
		const static size_t DATA_FILE_COUNT_AT = 0; // uint32_t - 4 Bytes
		const static size_t FREELIST_START_AT = 4; // DiskLoc - 8 Bytes
		const static size_t LAST_AVAIL_LOC_AT = 12; // DiskLoc - 8 Bytes
		const static size_t METADATA_COUNT_AT = 20; // uint32_t - 4 Bytes
	};
	class DbfsManager
	{
	public:
		// metadata's offset of fd/mmap array
		const static int META_OFFSET = -1;
		const static int MAX_META_SIZE = 100000; // Accurately: 104857.6
		const static size_t NAME_LENGTH = 128;
		const static size_t FILE_PER_DB = 32768;
		const static size_t BASE_SIZE = 16777216; // 16MiB
	private:
		char DBName[NAME_LENGTH];
		char RootPath[NAME_LENGTH];
		char TmpBuff[NAME_LENGTH * 8];
	public:
		int *fd, _fd[FILE_PER_DB]; // Array of file descriptors
		void **mem, *_mem[FILE_PER_DB]; // Array of start of memory addrs
		int32_t *DataFileCount; // Amount of data files
		Metadata *metas; // Total metas: 16MiB / 152B ~ 110000
		int32_t *MetadataCount; // Num of metadatas
		std::map<string, Metadata*> Metamap; // Metadata map
		DiskLoc *freelist; // First elem of FREELIST
		DiskLoc *lastAvail; // `MALLOC` at here (if no free node in freelist)
		using byte = char;
		DbfsManager(void)
		{
			throw std::runtime_error("`DbfsManager` requires all arguments");
		}
		DbfsManager(const char *_DBName, const char *_RootPath);
		~DbfsManager(void);
		void CreateNewDatafile(void);
		inline void* GetAddrFromDl(const DiskLoc &dl)
		{
			return (byte*)mem[dl.file] + dl.offset;
		}
		inline void* GetAddrFromDl_Safe(const DiskLoc &dl)
		{
			if(dl.file == DiskLoc::NullLoc)
				return nullptr;
			return (byte*)mem[dl.file] + dl.offset;
		}
	};
	inline void* GetAddrFromDl(const DbfsManager &mgr, const DiskLoc &dl)
	{
		using byte = char;
		return (byte*)mgr.mem[dl.file] + dl.offset;
	}
	inline void* GetAddrFromDl_Safe(const DbfsManager &mgr, const DiskLoc &dl)
	{
		using byte = char;
		if(dl.file == DiskLoc::NullLoc)
			return nullptr;
		return (byte*)mgr.mem[dl.file] + dl.offset;
	}
	size_t SizeOfDatafile(int index);
	DbfsManager CreateDatabase(const char *name, const char *root);
	namespace Allocator
	{
		DiskLoc MALLOC(DbfsManager &mgr, size_t size);
		void FREE(const DbfsManager &mgr, Record &rec);
	}
}
#endif // SHIKU_DBFS_MANAGER_HPP_