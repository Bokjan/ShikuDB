#include "ShikuDB.hpp"
#include "Literals.hpp"
string ShikuDB(string query)
{
	return ShikuDB(query.c_str());
}
string ShikuDB(const char *query)
{
	// Local variable becomes rvalue when returning
	return string("{\"ok\":true,\"message\":\"Under construction\"}");
}