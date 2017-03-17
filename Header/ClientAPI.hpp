#ifndef SHIKU_CLIENT_API_HPP_
#define SHIKU_CLIENT_API_HPP_
#include "Json.hpp"
#include <functional>
using Json = nlohmann::json;
namespace shiku
{
	std::function<void(Json&, Json&)> GetApiFuncByString(const char *str);
	void ShowDatabases(Json &query, Json &ret);
	void UseDatabase(Json &query, Json &ret);
	void InvalidOperation(Json &query, Json &ret);
}
#endif // SHIKU_CLIENT_API_HPP_