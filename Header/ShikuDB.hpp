#ifndef SHIKU_SHIKU_DB_HPP_
#define SHIKU_SHIKU_DB_HPP_
#include <string>
#include <stdexcept>
#include "Json.hpp"
namespace shiku
{
	using string = std::string;
	using Json = nlohmann::json;
	// string ShikuDB(string query);
	// string ShikuDB(const char *query);
	// A singleton class
	class ShikuDB
	{
	private:
	public:
		ShikuDB(void);
		string RunQuery(const char *query);
	};
}
#endif //SHIKU_SHIKU_DB_HPP_