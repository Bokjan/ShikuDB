#include "QueryQueue.hpp"
using namespace shiku;
namespace shiku
{
	// Singleton
	QueryQueue queryQueue;
}
QueryQueue::QueryQueue(void)
{
	counter = 0;
	mutex = false;
}
uint64_t QueryQueue::Push(void)
{
	queue.push(counter);
	return counter++;
}
void QueryQueue::Pop(void)
{
	queue.pop();
}
bool QueryQueue::TimeToProcess(uint64_t id)
{
	return mutex || queue.front() != id ? false : true;
}
bool QueryQueue::Lock(void)
{
	if(mutex)
		return false;
	return mutex = true;
}
bool QueryQueue::Unlock(void)
{
	if(!mutex)
		return false;
	return !(mutex = false);
}