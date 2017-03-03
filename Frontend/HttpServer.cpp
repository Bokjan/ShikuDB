#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "Literals.hpp"
#include "HttpServer.hpp"
namespace shiku
{
	static void EventHandler(mg_connection *c, int ev, void *ev_data);
	HttpServer::HttpServer(int port)
	{
		SetPort(port);
		mg_mgr_init(&mgr, NULL);
		c = mg_bind(&mgr, Port, EventHandler);
		if(c == NULL)
			throw std::runtime_error(HTTPD_PORT_LISTEN_FAILURE);
		mg_set_protocol_http_websocket(c);
		for( ; ;)
			mg_mgr_poll(&mgr, 1000);
	}
	HttpServer::~HttpServer(void)
	{
		mg_mgr_free(&mgr);
	}
	void HttpServer::SetPort(int port)
	{
		sprintf(Port, "%d", port);
	}
	static void EventHandler(mg_connection *c, int ev, void *ev_data)
	{
		//See definition of `http_message` at line 4072, mongoose.h
		switch(ev)
		{
			case MG_EV_HTTP_REQUEST:
				http_message *hm = (http_message*)ev_data;
				if(strcmp(hm->method.p, "POST"))
				{
					mg_send_head(c, 200, SIZEOF(HTTPD_INVALID_REQUEST_METHOD), "Content-Type: application/json");
					mg_printf(c, "%s", HTTPD_INVALID_REQUEST_METHOD);
					goto RETURN;
				}
				#define PROTOTYPING "{\"ok\":true,\"message\":\"Under construction\"}"
				mg_send_head(c, 200, SIZEOF(PROTOTYPING), "Content-Type: application/json");
				mg_printf(c, "%s", PROTOTYPING);
				break;
		}
	RETURN:;
	}
}