#include <cstdio>
#include <string>
#include <stdexcept>
#include "ShikuDB.hpp"
#include "Utility.hpp"
#include "Literals.hpp"
#include "HttpServer.hpp"
#include "QueryQueue.hpp"
using namespace shiku;
extern shiku::ShikuDB dbmgr;
namespace shiku
{
	using string = std::string;
	static void EventHandler(mg_connection *c, int ev, void *ev_data);
	HttpServer::HttpServer(int port)
	{
		SetPort(port);
		isShutdown = false;
		mg_mgr_init(&mgr, NULL);
		c = mg_bind(&mgr, Port, EventHandler);
		if(c == NULL)
		{
			Log.Fatal("Fail to listen on port %d", port);
			throw std::runtime_error(HTTPD_PORT_LISTEN_FAILURE);
		}
		mg_set_protocol_http_websocket(c);
		Log.Info("Listening on port %d", port);
	}
	HttpServer::~HttpServer(void)
	{
		mg_mgr_free(&mgr);
		Log.Info("Http server stopped");
	}
	void HttpServer::Run(void)
	{
		for( ; !isShutdown;)
			mg_mgr_poll(&mgr, 1000);
	}
	void HttpServer::Shutdown(void)
	{
		isShutdown = true;
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
				string query(hm->body.p, hm->body.len); // Duplicate the query string to avoid no terminator
				string result = dbmgr.RunQuery(query.c_str());
				mg_send_head(c, 200, result.size(), HTTPD_EXTRA_HEADERS);
				mg_printf(c, "%s", result.c_str());
				// Pop and Unlock 
				queryQueue.Pop();
				queryQueue.Unlock();
				break;
		}
	RETURN:;
	}
}