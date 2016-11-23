#include "common.h"
#include "LogManager.h"

using namespace std;

LogManager* LogManager::instance = 0;

LogManager::LogManager()
	:
	logStream(0)
{
	this->Initialize();
}


LogManager::~LogManager()
{
	this->Terminate();
}

LogManager& LogManager::GetInstance()
{
	if (LogManager::instance == 0)
	{
		LogManager::instance = new LogManager();
	}
	return (*LogManager::instance);
}

void LogManager::Initialize()
{
	this->logStream = new fstream("KRC2_GC.log", std::fstream::out);
}

void LogManager::LogLine(std::string text)
{
	(*this->logStream) << text << endl;
}

void LogManager::LogError(std::string err)
{
	(*this->logStream) << "E R R O R :  " << err << endl;
}

void LogManager::Terminate()
{
	this->logStream->close();
	delete this->logStream;
	this->logStream = 0;
}
