#include <cstring>
#include "Utility.hpp"
bool shiku::Utility::IsEndsWith(const char *str, const char *suffix)
{
	int len1 = strlen(str);
	int len2 = strlen(suffix);
	if(len1 < len2)
		return false;
	for(int i = 1; i <= len2; ++i)
		if(str[len1 - i] != suffix[len2 - i])
			return false;
	return true;
}
uint32_t shiku::Utility::BkdrHash(const char *str)
{
	static uint32_t seed = 131;
	uint32_t hash = 0;
	while(*str)
		hash = hash * seed + (*str++);
	return hash & 0x7FFFFFFF;
}