#ifndef SHIKU_QUERY_QUEUE_HPP_
#define SHIKU_QUERY_QUEUE_HPP_
#include <queue>
#include <cstdint>
namespace shiku
{
	// Not thread-safe
	// No locks when modifying queue
	class QueryQueue
	{
	private:
		bool mutex;
		uint64_t counter;
		std::queue<uint64_t> queue;
	public:
		QueryQueue(void);
		bool Lock(void);
		bool Unlock(void);
		uint64_t Push(void);
		void Pop(void);
		bool TimeToProcess(uint64_t id);
	};
	// Singleton
	extern QueryQueue queryQueue;
}
#endif //SHIKU_QUERY_QUEUE_HPP_