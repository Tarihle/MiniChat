///Client
//#include <Network/Network.h>
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

#include <iostream>
#include <fstream>

constexpr BYTE Exit_WSAStartup = 1'01;
constexpr BYTE Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

int main()
{
    ///* WSA Startup */
    WSADATA data;
    int result = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);

    if (result)
    {
        //reportWindowsError(TEXT("WSAStartup"), result);
        return Exit_WSAStartup;
    }

    struct addrinfo hints, * list;
    int status;

    memset(&hints, 0, sizeof hints);    /* Fill with 0s */
    hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;

    status = getaddrinfo("10.5.5.108", "8080", &hints, &list);
    if (status != 0)    /* getaddrinfo return 0 on success */
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    SOCKET sckt = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
    if (INVALID_SOCKET == sckt)
    {
        //reportWindowsError(TEXT("socket"), WSAGetLastError());
    }

    connect(sckt, list->ai_addr, (int)list->ai_addrlen);
    _tprintf(TEXT("Connecting to kevin\n"));

    char buf[1000001];
    int charCount = recv(sckt, buf, 1000001, 0);
    buf[charCount-1] = '\0';

    _tprintf(TEXT("%s\n"), buf);

    std::ofstream chatLogs;
    chatLogs.open("ChatLogs.txt");
    chatLogs << buf;
    chatLogs.close();

    char msg[7] = "Ye boi";
    int len, bytes_sent;

    len = (int)strlen(msg) + 1;
    bytes_sent = send(sckt, msg, len, 0);

    freeaddrinfo(list); /* free the linked list */

    ///* WSA Cleanup */
    result = WSACleanup();
    if (SOCKET_ERROR == result)
    {
        //reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
        return Exit_WSACleanup;
    }

    return 0;
}