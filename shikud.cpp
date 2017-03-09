#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <functional>
#include "HttpServer.hpp"
#include "DbfsManager.hpp"
void Initialize(int argc, char *argv[])
{
	shiku::HttpServer *hs = new shiku::HttpServer(6207);
}
int main(int argc, char *argv[])
{
	// printf("%lu %lu %lu\n", sizeof(shiku::DiskLoc), sizeof(shiku::Metadata), sizeof(shiku::Record));
	// std::function<void(int, char*[])> Delegates[] = 
	// {
	// 	Initialize
	// };
	// for(auto fun : Delegates)
	// 	fun(argc, argv);
	shiku::DbfsManager mgr = shiku::CreateDatabase("test", "./");
	mgr.CreateNewDatafile();
	return 0;
}