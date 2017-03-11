#ifndef SHIKU_HTTP_SERVER_HPP_
#define SHIKU_HTTP_SERVER_HPP_
#include "mongoose.h"
#include "ShikuDB.hpp"
namespace shiku
{
	class HttpServer
	{
	public:
		HttpServer(void) = default;
		HttpServer(int port);
		~HttpServer(void);
		void SetPort(int port);
	private:
		char Port[8];
		mg_mgr mgr;
		mg_connection *c;
	};
}
#endif //SHIKU_HTTP_SERVER_HPP_