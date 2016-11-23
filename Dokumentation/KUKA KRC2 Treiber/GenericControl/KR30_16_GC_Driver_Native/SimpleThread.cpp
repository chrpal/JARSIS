#include "common.h"
#include "SimpleThread.h"


SimpleThread::SimpleThread()
	:
	threadHandle(0),
	threadID(0),
	closeThread(false)
{
}


SimpleThread::~SimpleThread()
{
}

unsigned int __stdcall SimpleThread::StaticThreadMain(void* pParam)
{

	#ifdef WINDOWS
	SimpleThread *ptr = reinterpret_cast<SimpleThread*>(pParam);
	void* tmpThreadHandle = ptr->getThreadHandle();

	unsigned int resultCode = ptr->ThreadMain();

	//TerminateThread(ptr->getThreadHandle(), resultCode);
	CloseHandle(tmpThreadHandle);

	_endthreadex(resultCode);

	//ExitThread(resultCode);
	return resultCode;
	
	#else //LINUX
		
	unsigned int resultCode = ptr->ThreadMain();
	pthread_exit();
	return resultCode;

	#endif
}

void SimpleThread::StartThread()
{
	this->closeThread = false;
	//Create and start thread

	#ifdef WINDOWS
	this->threadHandle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadMain, (void*)this, 0, &threadID);
	#else //LINUX
	pthread_create(&this->threadHandle, NULL, StaticThreadMain, NULL);
	#endif
}

void SimpleThread::SoftStopThread()
{
	this->closeThread = true;
	this->Resume();
}

void SimpleThread::Suspend()
{
	SuspendThread(this->threadHandle);
}

void SimpleThread::Resume()
{
	ResumeThread(this->threadHandle);
}

void SimpleThread::SleepThread(unsigned int milliseconds)
{
	Sleep(milliseconds);
}

void* SimpleThread::getThreadHandle()
{
	return this->threadHandle;
}

unsigned long SimpleThread::getThreadID()
{
	return this->threadID;
}