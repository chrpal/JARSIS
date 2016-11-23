#ifndef TCPUDPCLIENTTHREAD_H
#define TCPUDPCLIENTTHREAD_H

#include "common.h"
#include "SimpleThread.h"
class TCPUDPServer;

class TCPUDPClientThread : public SimpleThread
{
protected:

	SOCKET clientSocket;

	TCPUDPServer* clParentServer;

	unsigned int ThreadMain(void);

public:
	TCPUDPClientThread();
	~TCPUDPClientThread();

	void SetParentServer(TCPUDPServer* clParent);
	void CloseConnection(void);
	void Send(const char* data, unsigned int len);

	#ifdef WINDOWS
	void SetClientSocket(SOCKET s);
	#endif
};

#endif