// #include <vector>
// #include <string>
#include <cstring>
#include "Utility.hpp"
#include "ShikuDB.hpp"
#include "Literals.hpp"
#include "ClientAPI.hpp"
using std::string;
extern shiku::ShikuDB dbmgr;
namespace shiku
{
	static inline bool CompareTwoStrings(const char *a, const char *b)
	{
		return strcmp(a, b) == 0;
	}
	std::function<void(Json&, Json&)> GetApiFuncByString(const char *str)
	{
		if(CompareTwoStrings(str, "ConnectionTest"))
			return ConnectionTest;
		if(CompareTwoStrings(str, "ShowDatabases"))
			return ShowDatabases;
		if(CompareTwoStrings(str, "UseDatabase"))
			return UseDatabase;
		return InvalidOperation;
	}
	void ConnectionTest(Json &query, Json &ret)
	{
		ret["ok"] = true;
	}
	void ShowDatabases(Json &query, Json &ret)
	{
		// Returns an array (field `databases`) 
		// which contains all the databases' name
		ret["databases"] = Json::array();
		for(auto &item : dbmgr.DBs)
			ret["databases"].push_back(item.first);
		ret["ok"] = true;
	}
	void UseDatabase(Json &query, Json &ret)
	{
		if(query.find("database") == query.end())
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Database name not provided";
			return;
		}
		// Set `ok` to true if queried DB exists
		if(dbmgr.DBs.find(query["database"]) != dbmgr.DBs.end())
			ret["ok"] = true;
		else
			ret["message"] = "Database not found";
	}
	void InvalidOperation(Json &query, Json &ret)
	{
		ret[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_INVALID_OPERATION;
	}
}