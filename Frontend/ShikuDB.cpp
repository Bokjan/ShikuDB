#include "ShikuDB.hpp"
#include "Literals.hpp"
namespace shiku
{
	extern Json config;
	extern ShikuDB dbmgr;
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
			// ShutdownServer
			if(input["operation"] == "shutdown")
			{
				extern bool shutdown;
				shutdown = true;
				output["ok"] = true;
				break;
			}
		}
		while(false);

		// Local variable becomes rvalue when returning
		return output.dump();
	}
}