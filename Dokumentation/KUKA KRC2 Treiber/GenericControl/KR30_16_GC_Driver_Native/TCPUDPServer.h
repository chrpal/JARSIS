#ifndef TCPUDPSERVER_H
#define TCPUDPSERVER_H

#include "common.h"
#include "SimpleThread.h"


class TCPUDPClientThread;
class TCPUDPDataReceivedDelegator;

class TCPUDPServer : public SimpleThread, public TCPUDPClientDelegator
{
protected:
	std::string sIpAddress;
	unsigned short usPort;
	unsigned int uiServerType;


	int iErrorCode;

	//Windows specific
	#ifdef WINDOWS
	WSADATA wsaData;
	#endif

	SOCKET ListenSocket;

	std::map<TCPUDPClientThread*, SOCKET> clientThreadToSocketMap;
	std::vector<TCPUDPDataReceivedDelegator*> dataReceivedDelegators;
	unsigned int ThreadMain(void);
	void InitializeServer(void);
	void TerminateServer(void);
	void StartServer(std::string IpAddress, unsigned short port);


public:

	static const int TCPUDPSERVER_ERROR_NONE;
	static const int TCPUDPSERVER_ERROR_WSASTARTUP;
	static const int TCPUDPSERVER_SERVER_TYPE_TCP;
	static const int TCPUDPSERVER_SERVER_TYPE_UDP;

	TCPUDPServer();
	~TCPUDPServer();
	
	void StartTCPServer(std::string IpAddress, unsigned short port);
	void StartUDPServer(std::string IpAddress, unsigned short port);
	void StopServer(void);

	bool ErrorOccurred(void);
	bool IsTCPServer(void);
	bool IsUDPServer(void);
	void RegisterDataReceivedDelegate(TCPUDPDataReceivedDelegator* delegator);

	void SendToAll(const char* data, unsigned int len);

	void OnClientDataReceived(char* buffer, unsigned int len, TCPUDPClientThread* clientThread);
	void OnClientConnectionClosed(TCPUDPClientThread* clientThread);
	void OnClientErrorOccurred(TCPUDPClientThread* clientThread);
	void OnClientSendErrorOccurred(TCPUDPClientThread* clientThread);
};

#endif