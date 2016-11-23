#include "common.h"
#include "TCPUDPServer.h"
#include "TCPUDPClientThread.h"
#include "TypeDefs.h"

using namespace std;

#define WINDOWS

const int TCPUDPServer::TCPUDPSERVER_ERROR_NONE = 0;
const int TCPUDPServer::TCPUDPSERVER_ERROR_WSASTARTUP = 1;

const int TCPUDPServer::TCPUDPSERVER_SERVER_TYPE_TCP = 0;
const int TCPUDPServer::TCPUDPSERVER_SERVER_TYPE_UDP = 1;

TCPUDPServer::TCPUDPServer()
	:
		sIpAddress("127.0.0.1"),
		usPort(6008),
		#ifdef WINDOWS
		ListenSocket(INVALID_SOCKET),
		#else
		ListenSocket(0),
		#endif	
		uiServerType(TCPUDPSERVER_SERVER_TYPE_TCP)
{
	
}


TCPUDPServer::~TCPUDPServer()
{
}

//Wait for incoming clients and create specific client threads
unsigned int TCPUDPServer::ThreadMain()
{
	this->closeThread = false;

	this->InitializeServer();

	SOCKET clientSocket = INVALID_SOCKET;

	#ifdef WINDOWS
	#else
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	#endif

	#ifdef WINDOWS
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(this->ListenSocket, &readSet);
	timeval timeout;
	timeout.tv_sec = 0;  // Zero timeout (poll)
	timeout.tv_usec = 0;
	#endif

	while (this->closeThread == false)
	{
		//Wait for incoming connections
		#ifdef WINDOWS

		/*if (select(this->ListenSocket, &readSet, NULL, NULL, &timeout) == 1)
		{*/
			clientSocket = accept(this->ListenSocket, NULL, NULL);
		//}

		//Build up a new client socket thread
		#else //LINUX
		clientSocket = accept(this->ListenerSocket, (struct sockaddr *)&cli_addr, &clilen);

		#endif

		TCPUDPClientThread* clientThread = new TCPUDPClientThread();
		clientThread->SetClientSocket(clientSocket);
		clientThread->SetParentServer(this);

		this->clientThreadToSocketMap[clientThread] = clientSocket;

		clientThread->StartThread();

		#ifdef WINDOWS
		clientSocket = INVALID_SOCKET;
		#else
		clientSocket = 0;
		#endif
	}
	this->TerminateServer();
	return 0;
}

void TCPUDPServer::InitializeServer()
{
	#ifdef WINDOWS
	this->ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;
	int iResult = 0;

	//First Initialize WSA
	iResult = WSAStartup(0x0202, &wsaData);
	if (iResult != 0)
	{
		//ERROR
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	//Check if TCP or UDP Server
	if (this->uiServerType == TCPUDPSERVER_SERVER_TYPE_TCP)
	{
		hints.ai_protocol = IPPROTO_TCP;
	}
	else if (this->uiServerType == TCPUDPSERVER_SERVER_TYPE_UDP)
	{
		hints.ai_protocol = IPPROTO_UDP;
	}

	hints.ai_flags = AI_PASSIVE;
	hints.ai_addr = INADDR_ANY;

	char* cPort = new char[64];
	_itoa(this->usPort, cPort, 10);

	iResult = getaddrinfo(NULL, cPort, &hints, &result);
	if (iResult != 0)
	{
		//ERROR
		return;
	}

	this->ListenSocket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{
		//ERROR
		return;
	}

	iResult = ::bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		//ERROR
		return;
	}

	freeaddrinfo(result);

	iResult = listen(this->ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(this->ListenSocket);
		WSACleanup();
		this->ListenSocket = 0;
		//ERROR
		return;
	}

	//Socket is initialized

	#else //LINUX

	struct sockaddr_in serv_addr;

	this->ListenerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->ListenerSocket < 0)
	{
		//ERROR
		return;
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(this->usPort);

	if (bind(this->ListenerSocket, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
	{
		//ERROR
		return;
	}
	listen(this->ListenerSocket,5);

	#endif
}

void TCPUDPServer::TerminateServer()
{
	//Close all client sockets
	std::map<TCPUDPClientThread*, SOCKET>::iterator it = this->clientThreadToSocketMap.begin();
	TCPUDPClientThread* clientThread = 0;
	while (this->clientThreadToSocketMap.size() > 0)
	{
		clientThread = it->first;
		clientThread->CloseConnection();
	}

	//Now cleanup Server
	#ifdef WINDOWS
	closesocket(this->ListenSocket);
	this->ListenSocket = INVALID_SOCKET;
	WSACleanup();
	#endif
}

void TCPUDPServer::StartServer(std::string IpAddress, unsigned short port)
{
	this->sIpAddress = IpAddress;
	this->usPort = port;

	this->StartThread();
}

void TCPUDPServer::StartTCPServer(std::string IpAddress, unsigned short port)
{
	this->uiServerType = TCPUDPSERVER_SERVER_TYPE_TCP;
	this->StartServer(IpAddress, port);
}

void TCPUDPServer::StartUDPServer(std::string IpAddress, unsigned short port)
{
	this->uiServerType = TCPUDPSERVER_SERVER_TYPE_UDP;
	this->StartServer(IpAddress, port);
}

void TCPUDPServer::StopServer(void)
{
	this->SoftStopThread();
}

bool TCPUDPServer::ErrorOccurred(void)
{
	return this->iErrorCode != 0;
}

bool TCPUDPServer::IsTCPServer(void)
{
	return this->uiServerType == TCPUDPSERVER_SERVER_TYPE_TCP;
}

bool TCPUDPServer::IsUDPServer(void)
{
	return this->uiServerType == TCPUDPSERVER_SERVER_TYPE_UDP;
}

void TCPUDPServer::RegisterDataReceivedDelegate(TCPUDPDataReceivedDelegator* delegator)
{
	this->dataReceivedDelegators.push_back(delegator);
}

void TCPUDPServer::SendToAll(const char* data, unsigned int len)
{
	for (std::map<TCPUDPClientThread*, SOCKET>::iterator it = this->clientThreadToSocketMap.begin(); it != this->clientThreadToSocketMap.end(); it++)
	{
		TCPUDPClientThread *clientThread = it->first;
		clientThread->Send(data, len);
	}
}

void TCPUDPServer::OnClientDataReceived(char* buffer, unsigned int len, TCPUDPClientThread* clientThread)
{
	if (this->clientThreadToSocketMap.count(clientThread) != 0)
	{
		for (std::vector<TCPUDPDataReceivedDelegator*>::iterator i = this->dataReceivedDelegators.begin(); i != this->dataReceivedDelegators.end(); i++)
		{
			//Delegate received buffer
			(*i)->OnTCPUDPDataReceived(buffer, len);
		}
	}
}

void TCPUDPServer::OnClientConnectionClosed(TCPUDPClientThread* clientThread)
{
	if (this->clientThreadToSocketMap.count(clientThread) != 0)
	{
		SOCKET clientSocket = this->clientThreadToSocketMap[clientThread];
		this->clientThreadToSocketMap.erase(clientThread);

		shutdown(clientSocket, SD_SEND);
		closesocket(clientSocket);

		delete clientThread;
	}
}

void TCPUDPServer::OnClientErrorOccurred(TCPUDPClientThread* clientThread)
{
	if (this->clientThreadToSocketMap.count(clientThread) != 0)
	{
		SOCKET clientSocket = this->clientThreadToSocketMap[clientThread];
		this->clientThreadToSocketMap.erase(clientThread);

		closesocket(clientSocket);

		delete clientThread;
	}
}

void TCPUDPServer::OnClientSendErrorOccurred(TCPUDPClientThread* clientThread)
{
	//Error during sending occurred
}