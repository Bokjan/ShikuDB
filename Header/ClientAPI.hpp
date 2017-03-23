#ifndef SHIKU_CLIENT_API_HPP_
#define SHIKU_CLIENT_API_HPP_
#include "Json.hpp"
#include <functional>
using Json = nlohmann::json;
namespace shiku::API
{
	std::function<void(Json&, Json&)> GetApiFuncByString(const char *str);
	void ConnectionTest(Json &query, Json &ret);
	void ShutdownServer(Json &query, Json &ret);
	void ShowDatabases(Json &query, Json &ret);
	void UseDatabase(Json &query, Json &ret);
	void CreateDatabase(Json &query, Json &ret);
	void DropDatabase(Json &query, Json &ret);
	void InvalidOperation(Json &query, Json &ret);
	void CreateCollection(Json &query, Json &ret);
	void ShowCollections(Json &query, Json &ret);
}
#endif // SHIKU_CLIENT_API_HPP_