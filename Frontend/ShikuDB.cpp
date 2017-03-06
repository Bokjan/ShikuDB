#include <stdexcept>
#include "Json.hpp"
#include "ShikuDB.hpp"
#include "Literals.hpp"
using namespace shiku;
using Json = nlohmann::json;
string shiku::ShikuDB(string query)
{
	return ShikuDB(query.c_str());
}
string shiku::ShikuDB(const char *query)
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