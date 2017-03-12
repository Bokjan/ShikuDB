#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>
#include "Json.hpp"
#include "ShikuDB.hpp"
#include "Utility.hpp"
#include "HttpServer.hpp"
#include "DbfsManager.hpp"
using Json = nlohmann::json;
Json config;
shiku::ShikuDB dbmgr;
void ParseArgs(int argc, char *argv[]);
void InitDB(int, char *[]);
void InitHttpServer(int, char *[]);
int main(int argc, char *argv[])
{
	std::function<void(int, char*[])> Delegates[] = 
	{
		ParseArgs, InitDB, InitHttpServer
	};
	for(auto fun : Delegates)
		fun(argc, argv);
	// shiku::CreateDatabase("test", "/Volumes/RamDisk/t/");
	return 0;
}
void ParseArgs(int argc, char *argv[])
{
	for(int i = 0; i < argc; ++i)
	{
		// Parameter header
		if(argv[i][0] != '-')
			continue;
		// -c	Config File
		// -p	Port
		switch(argv[i][1])
		{
			case 'c': // Config File
			{
				if(!shiku::Utility::IsFileExists(argv[++i]))
					throw std::runtime_error("Cannot open config file");
				std::ifstream in(argv[i]);
				try
				{
					in >> config;
					in.close();
				}
				catch(std::invalid_argument e)
				{
					throw std::runtime_error("Configuration parse failed");
				}
				break;
			}
			case 'p': // Port
			{
				int port;
				sscanf(argv[++i], "%d", &port);
				config["port"] = port;
				break;
			}
		}
	}
}
void InitDB(int argc, char *argv[])
{
	// Check if `config.root` exists
	if(config.find("root") == config.end())
		throw std::runtime_error("Root path not set");
	dbmgr.SetRoot(config["root"]);
	dbmgr.Initialize();
}
void InitHttpServer(int argc, char *argv[])
{
	shiku::HttpServer *hs;
	if(config.find("port") != config.end())
		hs = new shiku::HttpServer((int)config["port"]);
	else
		hs = new shiku::HttpServer(6207);
	hs->Run();
}