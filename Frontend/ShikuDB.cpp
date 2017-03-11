#include "ShikuDB.hpp"
#include "Literals.hpp"
namespace shiku
{
	// Singleton
	ShikuDB db;
	extern Json config;
	ShikuDB::ShikuDB(void)
	{
		throw std::runtime_error("Must pass the config file");
	}
	string ShikuDB::RunQuery(const char *query)
	{
		// We'll parse input string later
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
		}
		while(false);
		// Local variable becomes rvalue when returning
		return output.dump();
	}
}