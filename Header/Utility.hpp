#ifndef SHIKU_UTILITY_HPP_
#define SHIKU_UTILITY_HPP_
namespace shiku
{
namespace Utility
{
	bool IsFileExists(const char *file);
	bool IsFileWriteable(const char *file);
	bool IsEndsWith(const char *str, const char *suffix);
}
}
#endif //SHIKU_UTILITY_HPP_