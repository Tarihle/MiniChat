#include "PrivateNetwork.h"
#include "Network.h"

#include <WS2tcpip.h>

constexpr BYTE Exit_WSAStartup = 1'01;
constexpr BYTE Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

#define PORT "6698"
#define IP "localhost"

namespace Net
{
	Network* Network::m_Instance = nullptr;

	Network* Net::Network::GetInstance()
	{
		if (nullptr == m_Instance)
		{
			return new Network();
		}

		return m_Instance;
	}

	int Network::Startup()
	{
		///* WSA Startup */
		WSADATA data;
		int result = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);

		if (result)
		{
			//reportWindowsError(TEXT("WSAStartup"), result);
			return Exit_WSAStartup;
		}
	}

	int Network::Cleanup()
	{
		///* WSA Cleanup */
		int result = WSACleanup();
		if (SOCKET_ERROR == result)
		{
			//reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
			return Exit_WSACleanup;
		}
	}

	Network::~Network()
	{

	}

	//////////////////////////////////////////////////

	int Socket::NewSocket()
	{
		struct addrinfo hints;
		struct addrinfo* list;
		struct addrinfo* iter;
		int status;
		int newSocket;

		memset(&hints, 0, sizeof hints);    /* Fill with 0s */
		hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;

		status = getaddrinfo(IP, PORT, &hints, &list);
		if (status != 0)    /* getaddrinfo return 0 on success */
		{
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			return 2;
		}

		SOCKET newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			//reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		return newSocket;
	}

	Socket::~Socket()
	{

	}
}
