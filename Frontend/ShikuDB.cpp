#include "ShikuDB.hpp"
string ShikuDB(string query)
{
	return ShikuDB(query.c_str());
}
string ShikuDB(const char *query)
{
	return string("");
}