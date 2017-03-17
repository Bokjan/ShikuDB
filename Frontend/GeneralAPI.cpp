// #include <vector>
// #include <string>
#include <cstring>
#include "ShikuDB.hpp"
#include "ClientAPI.hpp"
using std::string;
extern shiku::ShikuDB dbmgr;
namespace shiku
{
	std::function<void(Json&, Json&)> GetApiFuncByString(const char *str)
	{
		#define STRCMP(a, b) (strcmp(a, b) == 0)
		if(STRCMP(str, "ShowDatabases"))
			return ShowDatabases;
		if(STRCMP(str, "UseDatabase"))
			return UseDatabase;
		return InvalidOperation;
	}
	void ShowDatabases(Json &query, Json &ret)
	{
		// Returns an array (field `databases`) 
		// which contains all the databases' name
		ret["databases"] = Json::array();
		for(auto &item : dbmgr.DBs)
			ret["databases"].push_back(item.first);
	}
	void UseDatabase(Json &query, Json &ret)
	{

	}
	void InvalidOperation(Json &query, Json &ret)
	{

	}
}