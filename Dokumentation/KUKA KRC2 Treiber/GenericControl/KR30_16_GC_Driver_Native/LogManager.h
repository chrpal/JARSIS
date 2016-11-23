#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "common.h"

class LogManager
{
protected:
	static LogManager* instance;
	std::fstream* logStream;
	LogManager();

public:
	~LogManager();

	static LogManager& GetInstance();
	void Initialize();
	void LogLine(std::string text);
	void LogError(std::string err);
	void Terminate();
};

#endif