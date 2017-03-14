#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "ShikuDB.hpp"
#include "Utility.hpp"
#include "Literals.hpp"
namespace shiku
{
	extern Json config;
	extern ShikuDB dbmgr;
	void ShikuDB::SetRoot(string r)
	{
		strcpy(root, r.c_str());
	}
	void ShikuDB::Initialize(void)
	{
		char tmp[1024];
		// Read directory
		DIR *root_dir;
		dirent *iterator;
		root_dir = opendir(root);
		while((iterator = readdir(root_dir)) != nullptr)
		{
			// Strange problem.
			// d_name will have a +4 offset when compiling with MinGW
			string dbname;
#ifdef __WIN32
			dbname = iterator->d_name - 4;
#else
			dbname = iterator->d_name;
#endif
			if(!Utility::IsEndsWith(dbname.c_str(), ".db"))
				continue;
			Log.Debug("Loading database %s", dbname.c_str());
			// Concatatenate this DB's dir path
			sprintf(tmp, "%s%s/", root, dbname.c_str());
			dbname = dbname.substr(0, dbname.length() - 3);
			DBs.insert(std::make_pair(dbname, DbfsManager(dbname.c_str(), tmp)));
		}
	}
	string ShikuDB::RunQuery(const char *query)
	{
		Json input;
		Json output = 
		{
			{"ok", false}
		};
		do
		{
			// Test if input string is a JSON
			try
			{
				input = Json::parse(query);
			}
			catch(std::invalid_argument e)
			{
				output[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_INVALID_JSON_STRING;
				break;
			}
			/*
				input:
				{
					"operation" : "a_string"
				}
			*/
			// Check if field `operation` exists, true => not found
			if(input.find("operation") == input.end())
			{
				output[SHIKUDB_JSON_FIELD_MESSAGE] = SHIKUDB_ERROR_MESSAGE_OPERATION_NOT_PROVIDED;
				break;
			}
		}
		while(false);
		// Local variable becomes rvalue when returning
		return output.dump();
	}
}