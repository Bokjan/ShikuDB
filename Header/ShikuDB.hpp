#ifndef SHIKU_SHIKU_DB_HPP_
#define SHIKU_SHIKU_DB_HPP_
#include <map>
#include <string>
#include <utility>
#include <stdexcept>
#include "Json.hpp"
#include "DbfsManager.hpp"
namespace shiku
{
	using string = std::string;
	using Json = nlohmann::json;
	// string ShikuDB(string query);
	// string ShikuDB(const char *query);
	// A singleton class
	class ShikuDB
	{
	public:
		char root[1024]; // root path
		std::map<string, DbfsManager> DBs;
		ShikuDB(void) = default;
		void Initialize(void);
		void SetRoot(string r);
		string RunQuery(const char *query);
	};
}
#endif //SHIKU_SHIKU_DB_HPP_