#ifndef SHIKU_HTTP_SERVER_HPP_
#define SHIKU_HTTP_SERVER_HPP_
#include <microhttpd.h>
namespace shiku
{
	const int DefaultPort = 6207;
	class HttpServer
	{
	public:
		HttpServer(void);
		HttpServer(int port);
		~HttpServer(void);
	private:
		int Port;
	};
}
#endif //SHIKU_HTTP_SERVER_HPP_