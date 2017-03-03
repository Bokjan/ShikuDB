#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "Json.hpp"
#include "HttpServer.hpp"
namespace shiku
{
	nlohmann::json json = {{"ok", false}, {"message", "Invalid request"}};
	static void EventHandler(mg_connection *c, int ev, void *ev_data);
	HttpServer::HttpServer(void)
	{
		HttpServer(DefaultPort);
	}
	HttpServer::HttpServer(int port)
	{
		SetPort(port);
		mg_mgr_init(&mgr, NULL);
		c = mg_bind(&mgr, Port, EventHandler);
		if(c == NULL)
			throw std::runtime_error("Fail to listen on specified port");
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
				mg_send_head(c, 200, json.dump().size(), "Content-Type: application/json");
				mg_printf(c, "%s", json.dump().c_str());
				printf("%s\n", hm->body.p);
				break;
		}
	}
}