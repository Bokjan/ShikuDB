// Database-Filesystem Manager
#include "Utility.hpp"
#include "DbfsManager.hpp"
using namespace shiku;
/*
	First 100 bytes of *.meta is reserved, layout:
	0-4: Data file count of this DB (uint32)
*/
namespace META_LAYOUT
{
	const static size_t DATA_FILE_COUNT = 0;
	const static size_t NS_START_AT = 100;
};
DbfsManager::DbfsManager(const char *_DBName, const char *_RootPath)
{
	strcpy(DBName, _DBName);
	strcpy(RootPath, _RootPath);
	// Test if metadata exists
	sprintf(TmpBuff, "%s%s.meta", RootPath, DBName);
	if(!Utility::IsFileWriteable(TmpBuff))
		throw std::runtime_error("Fail to load database metadata");
	// Initialize fd/mmap array
	fd = _fd - (META_OFFSET * 8); // META_OFFSET is a negative nubmer
	mem = _mem - (META_OFFSET * 8);
	// mmap metadata
	fd[META_OFFSET] = open(TmpBuff, O_RDWR);
	mem[META_OFFSET] = mmap(NULL, BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd[META_OFFSET], 0);
	// Load data files
	DataFileCount = (int*)((byte*)mem[META_OFFSET] + META_LAYOUT::DATA_FILE_COUNT);
	for(int i = 0; i < *DataFileCount; ++i)
	{
		sprintf(TmpBuff, "%s%s.%d", RootPath, DBName, i);
		if(!Utility::IsFileWriteable(TmpBuff))
			throw std::runtime_error("Fail to load database data");
		fd[i] = open(TmpBuff, O_RDWR);
		// .0 => 32MiB, .1 => 64MiB, ...
		// (0x1 << 5) == 32, 32MiB == 33554432KiB (33554432 = 32 * 1024 * 1024)
		// (0x1 << i + 5) << 10 << 10 = 0x1 << i + 25
		// The 7th file reached 2GiB
		size_t filesize = i >= 7 ? 0x80000000 /*2GiB*/ : BASE_SIZE << (i + 1);
		mem[i] = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd[i], 0);
	}
}
DbfsManager::~DbfsManager(void)
{
	// Sync data to files
	for(int i = 0; i < *DataFileCount; ++i)
	{
		size_t filesize = i >= 7 ? 0x80000000 /*2GiB*/ : BASE_SIZE << (i + 1);
		msync(mem[i], filesize, MS_SYNC);
		close(fd[i]);
	}
	msync(mem[META_OFFSET], BASE_SIZE, MS_SYNC);
	close(fd[META_OFFSET]);
}