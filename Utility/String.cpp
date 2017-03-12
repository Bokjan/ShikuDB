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