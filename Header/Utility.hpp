#ifndef SHIKU_UTILITY_HPP_
#define SHIKU_UTILITY_HPP_
#include <cstdio>
namespace shiku::Utility
{
	bool IsFileExists(const char *file);
	bool IsFileWriteable(const char *file);
	bool IsEndsWith(const char *str, const char *suffix);
	bool MakeDirectory(const char *path);
	bool DeleteFile(const char *path);
}
namespace shiku
{
	class LogClass
	{
	public:
		enum class Level
		{
			OFF = 0,
			Fatal,
			Warn,
			Info,
			Debug,
			All
		};
		LogClass(void);
		~LogClass(void);
		void SetFile(const char *file);
		void SetLevel(Level l);
		void Debug(const char *fmt, ...);
		void Info(const char *fmt, ...);
		void Warn(const char *fmt, ...);
		void Fatal(const char *fmt, ...);
	private:
		FILE *fp;
		Level level;
		void PrintTime(void);
	};
	extern LogClass Log;
}
#endif //SHIKU_UTILITY_HPP_