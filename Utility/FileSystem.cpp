#ifdef _WIN32 // Windows
#include <io.h>
#else
#include <unistd.h>
#endif // _WIN32
#include "Utility.hpp"
#ifdef _WIN32
bool shiku::Utility::IsFileExists(const char *file)
{
	return _access(file, 0)) != -1;
}
bool shiku::Utility::IsFileWriteable(const char *file)
{
	return _access(file, 2)) != -1;
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
#endif // _WIN32