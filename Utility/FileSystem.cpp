#ifdef _WIN32 // Windows
#include <io.h>
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif // _WIN32
#include <cstdio>
#include "Utility.hpp"
#ifdef _WIN32
bool shiku::Utility::IsFileExists(const char *file)
{
	return _access(file, 0) != -1;
}
bool shiku::Utility::IsFileWriteable(const char *file)
{
	return _access(file, 2) != -1;
}
bool shiku::Utility::MakeDirectory(const char *path)
{
	if(_mkdir(path) == 0)
		return true;
	return false;
}
bool shiku::Utility::DeleteFile(const char *path)
{
	return ::DeleteFileA(path);
}
#else
bool shiku::Utility::IsFileExists(const char *file)
{
	return access(file, F_OK) == 0;
}
bool shiku::Utility::IsFileWriteable(const char *file)
{
	return access(file, W_OK) == 0;
}
bool shiku::Utility::MakeDirectory(const char *path)
{
	if(mkdir(path, 0744) != -1)
		return true;
	return false;
}
bool shiku::Utility::DeleteFile(const char *path)
{
	return remove(path) == 0;
}
#endif // _WIN32