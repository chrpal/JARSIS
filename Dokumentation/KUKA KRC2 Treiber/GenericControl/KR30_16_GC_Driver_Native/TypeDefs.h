
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

class TCPUDPClientThread;

class TCPUDPDataReceivedDelegator
{
	public:
		virtual void OnTCPUDPDataReceived(char* data, unsigned int len) = 0;
};

class TCPUDPClientDelegator
{
	public:
		virtual void OnClientDataReceived(char* data, unsigned int len, TCPUDPClientThread *thread) = 0;
		virtual void OnClientConnectionClosed(TCPUDPClientThread* thread) = 0;
		virtual void OnClientErrorOccurred(TCPUDPClientThread* thread) = 0;
		virtual void OnClientSendErrorOccurred(TCPUDPClientThread* thread) = 0;
};

class KRC2Delegator
{
	public:
		virtual void OnMessageReceived() = 0;
		virtual void OnErrorOccurred() = 0;
};

#ifndef WINDOWS
typedef SOCKET int;
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#endif

#endif