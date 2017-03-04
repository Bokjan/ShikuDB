#ifndef SHIKU_QUERY_QUEUE_HPP_
#define SHIKU_QUERY_QUEUE_HPP_
#include <queue>
namespace shiku
{
	using uint64_t = unsigned long long;
	class QueryQueue
	{
	private:
		bool mutex;
		uint64_t counter;
		std::queue<uint64_t> queue;
	public:
		QueryQueue(void);
		inline bool Lock(void);
		inline bool Unlock(void);
		inline uint64_t Push(void);
		inline void Pop(void);
		inline bool TimeToProcess(uint64_t id);
	};
}
#endif //SHIKU_QUERY_QUEUE_HPP_