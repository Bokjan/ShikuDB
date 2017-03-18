#include <cstdio>
#include <utility>
#include <unistd.h>
#include <dirent.h>
#include <stdexcept>
#include <sys/types.h>
#include "Utility.hpp"
#include "ShikuDB.hpp"
#include "Literals.hpp"
#include "ClientAPI.hpp"
using std::string;
extern shiku::ShikuDB dbmgr;
namespace shiku::API
{
	void CreateDatabase(Json &query, Json &ret)
	{
		if(query.find("database") == query.end())
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_REQUIRED_FIELD_NOT_PROVIDED;
			return;
		}
		// Check if db name has a duplicate
		if(dbmgr.DBs.find(query["database"]) != dbmgr.DBs.end())
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Duplicated database name";
			return;
		}
		string dbname = query["database"];
		char dbpath[512];
		sprintf(dbpath, "%s%s.db/", dbmgr.root, dbname.c_str());
		if(!Utility::MakeDirectory(dbpath))
		{
			Log.Warn("Fail to create directory `%s`", dbpath);
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Fail to create new database";
			return;
		}
		try
		{
			dbmgr.DBs.insert(std::make_pair(dbname, shiku::CreateDatabase(dbname.c_str(), dbpath)));
		}
		catch(std::runtime_error e)
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Failed to create new database";
			return;
		}
		Log.Info("New database %s created and loaded", dbname.c_str());
		ret["ok"] = true;
	}
	void DropDatabase(Json &query, Json &ret)
	{
		if(query.find("database") == query.end())
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_REQUIRED_FIELD_NOT_PROVIDED;
			return;
		}
		// 1st: Destruct DbfsMgr
		string dbname = query["database"];
		auto itr = dbmgr.DBs.find(dbname);
		if(itr == dbmgr.DBs.end())
		{
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Database not found";
			return;
		}
		dbmgr.DBs.erase(itr);
		// 2nd: Delete files
		char dbpath[512], filepath[512];
		sprintf(dbpath, "%s%s.db/", dbmgr.root, dbname.c_str());
		DIR *root_dir;
		dirent *iterator;
		root_dir = opendir(dbpath);
		while((iterator = readdir(root_dir)) != nullptr)
		{
#ifdef __WIN32
			char *fname = iterator->d_name - 4;
#else
			char *fname = iterator->d_name;
#endif
			if(Utility::IsEndsWith(fname, ".")
				|| Utility::IsEndsWith(fname, ".."))
				continue;
			sprintf(filepath, "%s%s", dbpath, fname);
			if(remove(filepath) == 0)
				Log.Info("File %s deleted", filepath);
		}
		if(remove(dbpath) != 0)
		{
			Log.Warn("Database %s removed from dbmgr but files not deleted", dbname.c_str());
			ret[SHIKUDB_JSON_FIELD_MESSAGE] = "Fail to delete files";
			return;
		}
		Log.Warn("Database %s deleted", dbname.c_str());
		ret["ok"] = true;
	}
}