// Database-Filesystem Manager
#include "Utility.hpp"
#include "DbfsManager.hpp"
namespace shiku
{
	static size_t SizeOfDataFile(int index);
	DbfsManager CreateDatabase(const char *name, const char *root)
	{
		const size_t onemb = 0x1 << 20; // 1 MiB
		char buff[256];
		char *zero = new char[onemb];
		memset(zero, sizeof(zero), 0);
		// Write zeros to meta file
		sprintf(buff, "%s%s.meta", root, name);
		FILE *meta = fopen(buff, "wb");
		if(meta == nullptr)
			throw std::runtime_error("Fail to write metadata file");
		for(int i = 0; i < (0x1 << 24) / onemb; ++i)
			fwrite(zero, onemb, 1, meta);
		fclose(meta);
		// Write zeros to data file #0
		sprintf(buff, "%s%s.0", root, name);
		FILE *data1 = fopen(buff, "wb");
		if(data1 == nullptr)
			throw std::runtime_error("Fail to write data file #0");
		size_t sizeOfFile = SizeOfDataFile(0);
		for(int i = 0; i < sizeOfFile / onemb; ++i)
			fwrite(zero, onemb, 1, data1);
		fclose(data1);
		delete [] zero;
		// Initialize a `DbfsManager`
		DiskLoc dl;
		DbfsManager mgr(name, root);
		// Set `DataFileCount` to correct value (0)
		*mgr.DataFileCount = 1;
		// Set `lastAvail` to correct value (nothing to do)
		// Set `freelist` to NullLoc ({-1, 0})
		dl.file = DiskLoc::NullLoc;
		*mgr.freelist = dl;
		return mgr;
	}
	DiskLoc MALLOC(const DbfsManager &mgr, size_t size)
	{
		return DiskLoc();

	}
	void FREE_RECORD(const DbfsManager &mgr, Record &rec)
	{

	}
	static size_t SizeOfDataFile(int index)
	{
		// .0 => 32MiB, .1 => 64MiB, ...	
		// (0x1 << 5) == 32, 32MiB == 33554432 Bytes (33554432 = 32 * 1024 * 1024)
		// (0x1 << i + 5) << 10 << 10 = 0x1 << i + 25
		// The 7th file reached 2GiB
		return index >= 7 ? 0x1 << 31 /*2GiB*/ : 0x1 << (index + 25);
	}
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
		DataFileCount = (int32_t*)((byte*)mem[META_OFFSET] + META_LAYOUT::DATA_FILE_COUNT_AT);
		for(int i = 0; i < *DataFileCount; ++i)
		{
			sprintf(TmpBuff, "%s%s.%d", RootPath, DBName, i);
			if(!Utility::IsFileWriteable(TmpBuff))
				throw std::runtime_error("Fail to load database data");
			fd[i] = open(TmpBuff, O_RDWR);
			size_t filesize = SizeOfDataFile(i);
			mem[i] = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd[i], 0);
		}
		// Initialize metas' array (pointer)
		metas = (Metadata*)((byte*)mem[META_OFFSET] + META_LAYOUT::META_START_AT);
		// Initialize the freelist
		// and the last available `DiskLoc` position
		freelist = (DiskLoc*)((byte*)mem[META_OFFSET] + META_LAYOUT::FREELIST_START_AT);
		lastAvail = (DiskLoc*)((byte*)mem[META_OFFSET] + META_LAYOUT::LAST_AVAIL_LOC_AT);
	}
	DbfsManager::~DbfsManager(void)
	{
		// Sync data to files
		for(int i = 0; i < *DataFileCount; ++i)
		{
			size_t filesize = SizeOfDataFile(i);
			msync(mem[i], filesize, MS_SYNC);
			close(fd[i]);
		}
		msync(mem[META_OFFSET], BASE_SIZE, MS_SYNC);
		close(fd[META_OFFSET]);
	}
	void DbfsManager::CreateNewDatafile(void)
	{
		sprintf(TmpBuff, "%s%s.%u", RootPath, DBName, *DataFileCount);
		FILE *fp = fopen(TmpBuff, "wb");
		if(fp == nullptr)
			throw std::runtime_error("Fail to create new data file");
		const size_t onemb = 0x1 << 20; // 1 MiB
		size_t sizeOfFile = SizeOfDataFile(*DataFileCount);
		char *zero = new char[onemb];
		memset(zero, onemb, 0);
		for(int i = 0; i < sizeOfFile / onemb; ++i)
			fwrite(zero, onemb, 1, fp);
		// mmap it
		fd[*DataFileCount] = open(TmpBuff, O_RDWR);
		mem[*DataFileCount] = mmap(NULL, sizeOfFile, PROT_READ | PROT_WRITE, MAP_SHARED, fd[META_OFFSET], 0);
		// Modify this value at last due to a `-1` offset in operations above
		*DataFileCount += 1;
		printf("data file count = %d\n", *DataFileCount);
	}
}