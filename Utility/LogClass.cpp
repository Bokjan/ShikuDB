#include <ctime>
#include <cstdarg>
#include <stdexcept>
#include "Utility.hpp"
namespace shiku
{
	LogClass Log;
	LogClass::LogClass(void)
	{
		fp = stderr;
		level = Level::Info;
	}
	LogClass::~LogClass(void)
	{
		if(fp != nullptr)
			fclose(fp);
	}
	void LogClass::PrintTime(void)
	{
		time_t t = time(nullptr);
		tm *u = localtime(&t);
		fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d -", 
			1900 + u->tm_year, 1 + u->tm_mon, u->tm_mday, 
			u->tm_hour, u->tm_min, u->tm_sec);
	}
	void LogClass::SetFile(const char *file)
	{
		if(fp != nullptr)
			fclose(fp);
		fp = fopen(file, "a+");
		if(fp == nullptr)
			throw std::runtime_error("Fail to open log file!");
	}
	void LogClass::SetLevel(Level l)
	{
		level = l;
	}
	void LogClass::Debug(const char *fmt, ...)
	{
		if(level < Level::Debug)
			return;
		PrintTime();
		fprintf(fp, " [Debug] ");
		va_list ap;
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
		fputc('\n', fp);
		fflush(fp);
	}
	void LogClass::Info(const char *fmt, ...)
	{
		if(level < Level::Info)
			return;
		PrintTime();
		fprintf(fp, " [Info]  ");
		va_list ap;
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
		fputc('\n', fp);
		fflush(fp);
	}
	void LogClass::Warn(const char *fmt, ...)
	{
		if(level < Level::Warn)
			return;
		PrintTime();
		fprintf(fp, " [Warn]  ");
		va_list ap;
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
		fputc('\n', fp);
		fflush(fp);
	}
	void LogClass::Fatal(const char *fmt, ...)
	{
		if(level < Level::Fatal)
			return;
		PrintTime();
		fprintf(fp, " [Fatal] ");
		va_list ap;
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
		fputc('\n', fp);
		fflush(fp);
	}
}