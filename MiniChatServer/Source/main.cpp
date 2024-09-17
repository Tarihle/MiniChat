///Server
#include <Network/Network.h>
//
//int main()
//{
//	network();
//	
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <WS2tcpip.h>
#include <tchar.h>  /* _tprintf */

constexpr BYTE Exit_WSAStartup = 1'01;
constexpr BYTE Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

using namespace Net;

int main()
{
    short* error = new short(-1);
    Net::Socket server(error);

    server.NewSocketBind(1);

    server.Listening();
    server.Accepting();

    return 0;
}
