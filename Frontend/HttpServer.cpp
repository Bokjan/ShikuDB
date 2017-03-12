#include <cstdio>
#include <string>
#include <stdexcept>
#include "ShikuDB.hpp"
#include "Literals.hpp"
#include "HttpServer.hpp"
#include "QueryQueue.hpp"
#ifndef __WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
extern shiku::ShikuDB dbmgr;
namespace shiku
{
	using string = std::string;
	bool shutdown = false;
	static void EventHandler(mg_connection *c, int ev, void *ev_data);
	HttpServer::HttpServer(int port)
	{
		SetPort(port);
		mg_mgr_init(&mgr, NULL);
		c = mg_bind(&mgr, Port, EventHandler);
		if(c == NULL)
			throw std::runtime_error(HTTPD_PORT_LISTEN_FAILURE);
		mg_set_protocol_http_websocket(c);
	}
	HttpServer::~HttpServer(void)
	{
		mg_mgr_free(&mgr);
	}
	void HttpServer::SetPort(int port)
	{
		sprintf(Port, "%d", port);
	}
	void HttpServer::Run(void)
	{
		do
			mg_mgr_poll(&mgr, 1000);
		while(!shutdown);
		// Wait operations to finish for 1 second
#ifndef __WIN32
		sleep(1); // `sleep` in `unistd.h` takes param as second
#else
		Sleep(1000); // `Sleep` in `windows.h` takes param as millisecond
#endif
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
				string result = dbmgr.RunQuery(hm->body.p);
				mg_send_head(c, 200, result.size(), HTTPD_EXTRA_HEADERS);
				mg_printf(c, "%s", result.c_str());
				// Pop and Unlock 
				queryQueue.Pop();
				queryQueue.Unlock();
				// Send shutdown instruction if last operation is `shutdown`
				if(shutdown)
					shutdown = true;
				break;
		}
	RETURN:;
	}
}