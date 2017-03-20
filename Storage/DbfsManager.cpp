// Database-Filesystem Manager
#include "Utility.hpp"
#include "DbfsManager.hpp"
namespace shiku
{
	size_t SizeOfDatafile(int index);
	DbfsManager CreateDatabase(const char *name, const char *root)
	{
		Log.Info("Creating database %s", name);
		const size_t onemb = 0x1 << 20; // 1 MiB
		char buff[256];
		char *zero = new char[onemb];
		memset(zero, 0, onemb);
		// Write zeros to meta file
		sprintf(buff, "%s%s.meta", root, name);
		FILE *meta = fopen(buff, "wb");
		if(meta == nullptr)
		{
			Log.Fatal("Fail to write %s", buff);
			throw std::runtime_error("Fail to write metadata file");
		}
		for(int i = 0; i < (int)(DbfsManager::BASE_SIZE / onemb); ++i)
			fwrite(zero, onemb, 1, meta);
		fclose(meta);
		delete[] zero;
		Log.Info("New file %s created, size: %u", buff, DbfsManager::BASE_SIZE);
		// Initialize a `DbfsManager`
		DiskLoc dl;
		DbfsManager mgr(name, root);
		// Set `DataFileCount` to correct value ()
		*mgr.DataFileCount = 0;
		// Create the first data file
		mgr.CreateNewDatafile();
		// Set `lastAvail` to correct value (nothing to do)
		// Set `freelist` to NullLoc ({-1, 0})
		dl.file = DiskLoc::NullLoc;
		*mgr.freelist = dl;
		return mgr;
	}
	DbfsManager::DbfsManager(const char *_DBName, const char *_RootPath)
	{
		strcpy(DBName, _DBName);
		strcpy(RootPath, _RootPath);
		// Test if metadata exists
		sprintf(TmpBuff, "%s%s.meta", RootPath, DBName);
		if(!Utility::IsFileWriteable(TmpBuff))
		{
			Log.Fatal("Fail to load %s", TmpBuff);
			throw std::runtime_error("Fail to load database metadata");
		}
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
			{
				Log.Fatal("Fail to load %s", TmpBuff);
				throw std::runtime_error("Fail to load database data");
			}
			fd[i] = open(TmpBuff, O_RDWR);
			size_t filesize = SizeOfDatafile(i);
			mem[i] = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd[i], 0);
		}
		// Initialize metas' array (pointer)
		metas = (Metadata*)((byte*)mem[META_OFFSET] + META_LAYOUT::META_START_AT);
		// Initialize meta map
		MetadataCount = (int32_t*)((byte*)mem[META_OFFSET] + META_LAYOUT::METADATA_COUNT_AT);
		for(int i = 0; i < *MetadataCount; ++i)
			Metamap.insert(std::make_pair(metas[i].Name, &metas[i]));
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
			size_t filesize = SizeOfDatafile(i);
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
		{
			Log.Fatal("Fail to create %s", TmpBuff);
			throw std::runtime_error("Fail to create new data file");
		}
		Log.Debug("Creating new data file %s", TmpBuff);
		const size_t onemb = 0x1 << 20; // 1 MiB
		// `DataFileCount` refers to the index of data file here
		size_t sizeOfFile = SizeOfDatafile(*DataFileCount);
		char *zero = new char[onemb];
		memset(zero, 0, onemb);
		for(int i = 0; i < (int)(sizeOfFile / onemb); ++i)
			fwrite(zero, onemb, 1, fp);
		// mmap it
		fd[*DataFileCount] = open(TmpBuff, O_RDWR);
		mem[*DataFileCount] = mmap(NULL, sizeOfFile, PROT_READ | PROT_WRITE, MAP_SHARED, fd[META_OFFSET], 0);
		// Modify this value at last due to a `-1` offset in operations above
		*DataFileCount += 1;
		delete[] zero;
		Log.Info("New data file %s created, size: %lu", TmpBuff, sizeOfFile);
	}
}