#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "HttpServer.hpp"
namespace shiku
{
	HttpServer::HttpServer( void )
	{
		HttpServer( DefaultPort );
	}
	HttpServer::HttpServer( int port )
	{
		Port = port;
		
	}
	HttpServer::~HttpServer( void )
	{
		
	}
	static int RequestHandler()
	{

	}
}