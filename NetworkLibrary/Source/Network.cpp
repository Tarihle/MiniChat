#include "PrivateNetwork.h"
#include "Network.h"

#include <WS2tcpip.h>

constexpr BYTE Exit_WSAStartup = 1'01;
constexpr BYTE Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

namespace Net
{
	Network* Network::m_Instance = nullptr;

	Network::Network()
	{

	}

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
}
