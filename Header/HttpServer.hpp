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
		void Run(void);
		void Shutdown(void);
	private:
		char Port[8];
		mg_mgr mgr;
		mg_connection *c;
		bool isShutdown;
	};
}
#endif //SHIKU_HTTP_SERVER_HPP_