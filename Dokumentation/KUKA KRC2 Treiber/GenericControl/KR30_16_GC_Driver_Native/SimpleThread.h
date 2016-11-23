#ifndef SIMPLETHREAD_H
#define SIMPLETHREAD_H

class SimpleThread
{
protected:
	#ifdef WINDOWS
	void* threadHandle;
	#else
	pthread_t threadHandle;
	#endif
	unsigned int threadID;
	bool closeThread;

	static unsigned int __stdcall StaticThreadMain(void* pParam);
	virtual unsigned int ThreadMain() = 0;

public:
	SimpleThread(void);
	~SimpleThread(void);
	void StartThread(void);
	void SoftStopThread(void);
	void Suspend(void);
	void Resume(void);
	void SleepThread(unsigned int milliseconds);

	void* getThreadHandle();
	unsigned long getThreadID();
};

#endif