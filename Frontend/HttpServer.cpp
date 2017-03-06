#include <cstdio>
#include <string>
#include <stdexcept>
#include "ShikuDB.hpp"
#include "Literals.hpp"
#include "HttpServer.hpp"
#include "QueryQueue.hpp"
using namespace shiku;
using string = std::string;
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
static void EventHandler(mg_connection *c, int event, void *ev_data)
{
	// See definition of `http_message` at line 4072, mongoose.h
	switch(event)
	{
		case MG_EV_HTTP_REQUEST:
			http_message *hm = (http_message*)ev_data;
			if(hm->method.p[0] != 'P' || hm->method.p[1] != 'O')
			{
				mg_send_head(c, 200, _SIZEOF(HTTPD_INVALID_REQUEST_METHOD), "Content-Type: application/json");
				mg_printf(c, "%s", HTTPD_INVALID_REQUEST_METHOD);
				goto RETURN;
			}
			/// !!! This part is not thread-safe !!! ///
			uint64_t requestId = queryQueue.Push();
			// Block the request until time to process
			while(!queryQueue.TimeToProcess(requestId));
			// Lock
			queryQueue.Lock();
			// Process
			// Move semantics:
			// Call `string`'s constructor with rvalue reference
			// when receiving an rvalue returns by `ShikuDB`
			string result = ShikuDB(hm->body.p);
			mg_send_head(c, 200, result.size(), HTTPD_EXTRA_HEADERS);
			mg_printf(c, "%s", result.c_str());
			// Pop and Unlock 
			queryQueue.Pop();
			queryQueue.Unlock();
			break;
	}
RETURN:;
}