#include <map>
#include "Utility.hpp"
#include "ShikuDB.hpp"
#include "Literals.hpp"
#include "ClientAPI.hpp"
using std::string;
extern shiku::ShikuDB dbmgr;
namespace shiku::API
{
	std::function<void(Json&, Json&)> GetApiFuncByString(const char *str)
	{
		static const std::map<string, std::function<void(Json&, Json&)>> map = 
		{
			{"ConnectionTest", ConnectionTest},
			{"ShowDatabases", ShowDatabases},
			{"UseDatabase", UseDatabase},
			{"CreateDatabase", CreateDatabase},
			{"DropDatabase", DropDatabase}
		};
		auto ret = map.find(str);
		if(ret == map.end())
			return InvalidOperation;
		return ret->second;
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
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_REQUIRED_FIELD_NOT_PROVIDED;
			return;
		}
		// Set `ok` to true if queried DB exists
		if(dbmgr.DBs.find(query["database"]) != dbmgr.DBs.end())
			ret["ok"] = true;
		else
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Database not found";
	}
	void InvalidOperation(Json &query, Json &ret)
	{
		ret[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_INVALID_OPERATION;
	}
}