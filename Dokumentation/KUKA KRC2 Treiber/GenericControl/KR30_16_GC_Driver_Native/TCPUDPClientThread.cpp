#include "common.h"
#include "TCPUDPClientThread.h"
#include "TCPUDPServer.h"


TCPUDPClientThread::TCPUDPClientThread()
	:
	clientSocket(INVALID_SOCKET),
	clParentServer(0)
{
}


TCPUDPClientThread::~TCPUDPClientThread()
{
}


unsigned int TCPUDPClientThread::ThreadMain(void)
{
	bool connectionClosing = false;
	int iResult = 1;
	char recvbuf[4096] = { 0 };

	while (this->closeThread == false && connectionClosing == false && iResult > 0)
	{
		iResult = recv(this->clientSocket, recvbuf, sizeof(recvbuf), 0);

		if (iResult > 0)
		{
			//Notify TCPServer about incoming message
			this->clParentServer->OnClientDataReceived(recvbuf, sizeof(recvbuf), this);
			std::memset(recvbuf, 0, sizeof(recvbuf));
		}
		if (iResult == 0)
		{
			connectionClosing = true;
		}
	}

	if (connectionClosing == true)
	{
		this->clParentServer->OnClientConnectionClosed(this);
	}
	if (iResult < 0)
	{
		this->clParentServer->OnClientErrorOccurred(this);
	}
	return 0;
}

void TCPUDPClientThread::SetParentServer(TCPUDPServer* clParent)
{
	this->clParentServer = clParent;
}

void TCPUDPClientThread::CloseConnection(void)
{
	this->SoftStopThread();
}

void TCPUDPClientThread::Send(const char* data, unsigned int len)
{
	int iSentBytes = 0;
	while (iSentBytes != SOCKET_ERROR && (len-iSentBytes) > 0)
	{
		iSentBytes += ::send(this->clientSocket, data, len - iSentBytes, 0);
	}
	if (iSentBytes == SOCKET_ERROR)
	{
		this->clParentServer->OnClientSendErrorOccurred(this);
	}
}

void TCPUDPClientThread::SetClientSocket(SOCKET s)
{
	this->clientSocket = s;
}