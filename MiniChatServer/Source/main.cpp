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
    /////* WSA Startup */
    //WSADATA data;
    //int result = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);

    //if (result)
    //{
    //    //reportWindowsError(TEXT("WSAStartup"), result);
    //    return Exit_WSAStartup;
    //}

    //struct addrinfo hints, *list, *iter;
    //int status;
    //char ipstr[INET6_ADDRSTRLEN];   /* IP string */

    //memset(&hints, 0, sizeof hints);    /* Fill with 0s */
    //hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_flags = AI_PASSIVE;
    //hints.ai_protocol = IPPROTO_TCP;

    //status = getaddrinfo("localhost", "6667", &hints, &list);
    //if (status != 0)    /* getaddrinfo return 0 on success */
    //{
    //    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    //    return 2;
    //}

    short* error = new short(-1);
    Net::Socket server(error);

#pragma region PRINTADDR

    //_tprintf(TEXT("IP addresses for localhost:\n\n"));
    //for (iter = list; iter != NULL; iter = iter->ai_next) 
    //{
    //    void* addr;
    //    const char* ipver;

    //    // get the pointer to the address itself,
    //    // different fields in IPv4 and IPv6:
    //    if (iter->ai_family == AF_INET) 
    //    { // IPv4
    //        struct sockaddr_in* ipv4 = (struct sockaddr_in*)iter->ai_addr;
    //        addr = &(ipv4->sin_addr);
    //        ipver = "IPv4";
    //    }
    //    else 
    //    { // IPv6
    //        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)iter->ai_addr;
    //        addr = &(ipv6->sin6_addr);
    //        ipver = "IPv6";
    //    }

    //    // convert the IP to a string and print it:
    //    inet_ntop(iter->ai_family, addr, ipstr, sizeof ipstr);
    //    _tprintf(TEXT("  %s: %s\n"), ipver, ipstr);
    //}

#pragma endregion

    //SOCKET sckt = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
    //if (INVALID_SOCKET == sckt)
    //{
    //    //reportWindowsError(TEXT("socket"), WSAGetLastError());
    //}

    //if (SOCKET_ERROR == bind(sckt, list->ai_addr, (int)list->ai_addrlen))
    //{
    //    //reportWindowsError(TEXT("bind"), WSAGetLastError());
    //}

    server.NewSocketBind();

    //listen(sckt, 5);
    server.Listening();

    //struct sockaddr_storage their_addr;
    //socklen_t addr_size;
    //SOCKET new_fd;

    //addr_size = sizeof their_addr;
    //new_fd = accept(sckt, (struct sockaddr*)&their_addr, &addr_size);
    //if (INVALID_SOCKET != new_fd)
    //{
    //    _tprintf(TEXT("yay"));
    //}
    //else
    //{
    //    _tprintf(TEXT("nay"));
    //}

    server.Accepting();

    //freeaddrinfo(list); /* free the linked list */

    /////* WSA Cleanup */
    //result = WSACleanup();
    //if (SOCKET_ERROR == result)
    //{
    //    //reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
    //    return Exit_WSACleanup;
    //}

    return 0;
}
