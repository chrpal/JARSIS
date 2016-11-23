// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <fstream>
#include "TypeDefs.h"


#ifdef WINDOWS

#include "targetver.h"

#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#else //LINUX

#include<pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#endif

#endif
