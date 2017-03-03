#include <cstdlib>
#include <iostream>
#include <functional>
#include "HttpServer.hpp"
void Initialize(int argc, char *argv[])
{
	shiku::HttpServer *hs = new shiku::HttpServer(6207);
}
int main(int argc, char *argv[])
{
	std::function<void(int, char*[])> Delegates[] = 
	{
		Initialize
	};
	for(auto fun : Delegates)
		fun(argc, argv);
	return 0;
}