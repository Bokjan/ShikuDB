#include "QueryQueue.hpp"
using namespace shiku;
QueryQueue::QueryQueue(void)
{
	counter = 0;
	mutex = false;
}
inline uint64_t QueryQueue::Push(void)
{
	queue.push(counter);
	return counter++;
}
inline void QueryQueue::Pop(void)
{
	queue.pop();
}
inline bool QueryQueue::TimeToProcess(uint64_t id)
{
	return mutex || queue.front() != id ? false : true;
}
inline bool QueryQueue::Lock(void)
{
	if(mutex)
		return false;
	return mutex = true;
}
inline bool QueryQueue::Unlock(void)
{
	if(!mutex)
		return false;
	return !(mutex = false);
}