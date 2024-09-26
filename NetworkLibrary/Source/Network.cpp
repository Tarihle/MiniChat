#include "ErrorHandling.hpp"
#include "PrivateNetwork.h"

constexpr short Exit_WSAStartup = 1'01;
constexpr short Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

namespace Net
{
	Network* Network::m_Instance = nullptr;

	Network::Network(short*& errorOutput)
	{
#ifdef UNICODE
		_setmode(_fileno(stdout), _O_U16TEXT);	/* Sets unicode UTF16 character mode */
#else
		DWORD codePage;
		if (GetLocaleInfoEx(L"Fr-FR", LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&codePage, 2))
		{
			SetConsoleOutputCP(1252);	/* Sets code page to western europ latin when in ANSI */
		}
		else
		{
			reportWindowsError(TEXT("GetLocaleInfoEx"), GetLastError());
		}
#endif

		///* WSA Startup */
		WSADATA data;
		int result = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);

		if (result) /* if result == 0, startup succeeded*/
		{
			//reportWindowsError(TEXT("WSAStartup"), result);
			*errorOutput = Exit_WSAStartup;
		}

		*errorOutput = 0;
	}

	Network* Network::GetInstance(short*& errorOutput)
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new Network(errorOutput);
		}

		return m_Instance;
	}

	std::vector<pollfd>& Network::GetPollfds()
	{
		return m_Pollfds;
	}

	int Network::AddPollfd(SOCKET& newSocket)
	{
		pollfd newPollfd;

		newPollfd.fd = newSocket;
		newPollfd.events = POLLIN;

		m_Pollfds.push_back(newPollfd);

		return (int)(m_Pollfds.size() - 1);
	}

	Network::~Network()
	{
		///* WSA Cleanup */
		int result = WSACleanup();
		if (SOCKET_ERROR == result)
		{
			reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
			//return Exit_WSACleanup;
		}
	}

	//////////////////////////////////////////////////

	Socket::Socket(short*& errorOutput)
	{
		m_Network = Network::GetInstance(errorOutput);
	}

	void Socket::NewSocket(const char* IPAddress, const char* port,short optionalPrint)
	{		
		struct addrinfo hints;
		struct addrinfo* list;
		int status;
		SOCKET newSocket;

		memset(&hints, 0, sizeof hints);    /* Fill with 0s */
		hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;

		status = getaddrinfo(IPAddress, port, &hints, &list);
		if (status != 0)    /* getaddrinfo return 0 on success */
		{
			//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			reportWindowsError(TEXT("getaddrinfo"), WSAGetLastError());
			//return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		freeaddrinfo(list); /* free the linked list */

		((Network*)m_Network)->AddPollfd(newSocket);
	}

	void Socket::NewSocketBind(const char* IPAddress, const char* port, short optionalPrint)
	{
		struct addrinfo hints;
		struct addrinfo* list;
		int status;
		SOCKET newSocket;

		memset(&hints, 0, sizeof hints);    /* Fill with 0s */
		hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;

		status = getaddrinfo(IPAddress, port, &hints, &list);
		if (status != 0)    /* getaddrinfo return 0 on success */
		{
			reportWindowsError(TEXT("getaddrinfo"), WSAGetLastError());
			//return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		int idx = ((Network*)m_Network)->AddPollfd(newSocket);

		if (SOCKET_ERROR == bind(((Network*)m_Network)->GetPollfds()[idx].fd, list->ai_addr, (int)list->ai_addrlen))
		{
			reportWindowsError(TEXT("bind"), WSAGetLastError());
		}

		m_Handle = WSACreateEvent();
		WSAEventSelect(((Network*)m_Network)->GetPollfds()[idx].fd, m_Handle, FD_READ);

		freeaddrinfo(list); /* free the linked list */
	}

	void Socket::NewSocketConnect(const char* IPAddress, const char* port, short optionalPrint)
	{
		struct addrinfo hints;
		struct addrinfo* list;
		int status;
		SOCKET newSocket;

		memset(&hints, 0, sizeof hints);    /* Fill with 0s */
		hints.ai_family = AF_UNSPEC;    /* AF_INET or AF_INET6 to force version */
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;

		status = getaddrinfo(IPAddress, port, &hints, &list);
		if (status != 0)    /* getaddrinfo return 0 on success */
		{
			reportWindowsError(TEXT("getaddrinfo"), WSAGetLastError());
			//return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		int idx = ((Network*)m_Network)->AddPollfd(newSocket);

		status = connect(((Network*)m_Network)->GetPollfds()[idx].fd, list->ai_addr, (int)list->ai_addrlen);

		if (SOCKET_ERROR == status)
		{
			reportWindowsError(TEXT("connect"), WSAGetLastError());
		}

		m_Handle = WSACreateEvent();
		WSAEventSelect(((Network*)m_Network)->GetPollfds()[idx].fd, m_Handle, FD_READ);

		freeaddrinfo(list); /* free the linked list */
	}

	void* Socket::GetAddr(sockaddr* SocketAddress)
	{
		if (SocketAddress->sa_family == AF_INET) {
			return &(((struct sockaddr_in*)SocketAddress)->sin_addr);
		}

		return &(((struct sockaddr_in6*)SocketAddress)->sin6_addr);
	}

	void Socket::Listening()
	{
		listen(((Network*)m_Network)->GetPollfds()[0].fd, 5);
	}

	void Socket::Accepting()
	{
		struct sockaddr_storage their_addr;
		socklen_t addr_size;
		SOCKET new_fd;

		addr_size = sizeof their_addr;
		new_fd = accept(((Network*)m_Network)->GetPollfds()[0].fd, (struct sockaddr*)&their_addr, &addr_size);
		if (INVALID_SOCKET != new_fd)
		{
			consolePrint(TEXT("New client connected\n"));
		}
		else
		{
			reportWindowsError(TEXT("accept"), WSAGetLastError());
		}
	}

	void Socket::Close()
	{
		shutdown(((Network*)m_Network)->GetPollfds()[0].fd, SD_BOTH);
	}

	void Socket::Send(LPCTSTR buf, int len)
	{
		send(((Network*)m_Network)->GetPollfds()[0].fd, (char*)buf, (len + 1) * sizeof(TCHAR), 0); /* +1 because we will force \0 on the last character */
	}

	void Socket::Send(LPCTSTR buf, int len, unsigned __int64 destination)
	{
		send(destination, (char*)buf, (len + 1) * sizeof(TCHAR), 0); /* +1 because we will force \0 on the last character */
	}

	void Socket::SendAll(LPCTSTR buf, int len)
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();

		// We got some good data from a client
		for (int j = 1; j < pfds.size(); j++) /* Beginning at 1 to avoid sending to listener */
		{
			// Send to everyone!
			SOCKET destination = pfds[j].fd;

			if (SOCKET_ERROR == send(destination, (char*)buf, (len + 1) * sizeof(TCHAR), 0)) /* +1 because we will force \0 on the last character */
			{
				reportWindowsError(TEXT("SendAll"), WSAGetLastError());
			}
		}
	}

	void Socket::SendAll(LPCTSTR buf, int len, unsigned __int64 unwantedDestination)
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();

		// We got some good data from a client
		for (int j = 1; j < pfds.size(); j++)
		{
			// Send to everyone!
			SOCKET destination = pfds[j].fd;

			// Except the serverListener and ourselves
			if (destination != unwantedDestination)
			{
				if (SOCKET_ERROR == send(destination, (char*)buf, (len + 1) * sizeof(TCHAR), 0)) /* +1 because we will force \0 on the last character */
				{
					reportWindowsError(TEXT("sendAll"), WSAGetLastError());
				}
			}
		}
	}

	void Socket::PollLoop(std::function<void(unsigned __int64&, TCHAR*, Socket&)> connect, 
						  std::function<TSTR(TCHAR*, unsigned __int64&)> receive,
						  std::function<void(unsigned __int64)> disconnect)
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();
		SOCKET serverListener = ((Network*)m_Network)->GetPollfds()[0].fd;

		struct sockaddr_storage remoteAddr; // Client address
		char remoteIP[INET6_ADDRSTRLEN];

		socklen_t addrLength;
		SOCKET newFd;
		char buf[256];    // Buffer for client data

		for (;;) 
		{
			int poll_count = WSAPoll(&pfds[0], (ULONG)pfds.size(), -1);

			if (SOCKET_ERROR == poll_count) 
			{
				reportWindowsError(TEXT("poll"), WSAGetLastError());
				exit(1);
			}

			// Run through the existing connections looking for data to read
			for (int i = 0; i < pfds.size(); i++) 
			{
				// Check if someone's ready to read
				if (pfds[i].revents & POLLIN) 
				{ // We got one!!
					if (pfds[i].fd == serverListener) 
					{
						// If serverListener is ready to read, handle new connection
						addrLength = sizeof remoteAddr;
						newFd = accept(serverListener, (struct sockaddr*)&remoteAddr, &addrLength);

						if (INVALID_SOCKET == newFd) 
						{
							reportWindowsError(TEXT("accept"), WSAGetLastError());
						}
						else 
						{
							((Network*)m_Network)->AddPollfd(newFd);

							consolePrint(TEXT("pollserver: new connection from %1!s! on socket %2!llu!\n"),
								inet_ntop(remoteAddr.ss_family, GetAddr((struct sockaddr*)&remoteAddr), remoteIP, INET6_ADDRSTRLEN), 
								newFd);
							consolePrint(TEXT("Numbers of sockets check: %1!d!\n"), (int)pfds.size());

							recv(pfds[pfds.size() - 1].fd, buf, sizeof buf, 0);

							OnConnect(connect, newFd, (TCHAR*)buf);
						}
					}
					else 
					{
						// If not the serverListener, we're just a regular client
						int recvBytes = recv(pfds[i].fd, buf, sizeof buf, 0);

						SOCKET sender = pfds[i].fd;

						if (recvBytes == 0) 
						{
							// Connection closed
							consolePrint(TEXT("pollserver: socket %1!llu! hung up\n"), sender);

							closesocket(pfds[i].fd); // Bye!
							pfds.erase(pfds.begin() + i);
						}
						else if (recvBytes < 0)
						{
							reportWindowsError(TEXT("recv"), WSAGetLastError());
						}
						else 
						{
							buf[recvBytes - 1] = '\0';
							TSTR treatedData = OnServerReceive(receive, (TCHAR*)buf, sender);

							consolePrint(TEXT("%1!s!\n"), treatedData.c_str());

							// We got some good data from a client
							for (int j = 0; j < pfds.size(); j++) 
							{
								// Send to everyone!
								SOCKET destination = pfds[j].fd;

								// Except the serverListener and ourselves
								if (destination != serverListener && destination != sender) 
								{
									if (SOCKET_ERROR == send(destination, (char*)treatedData.c_str(), (int)(treatedData.size() + 1) * sizeof(TCHAR), 0))
									{
										reportWindowsError(TEXT("send"), WSAGetLastError());
									}
								}
							}
						}
					} // END handle data from client
				} // END got ready-to-read from poll()
				else if (pfds[i].revents & POLLERR || pfds[i].revents & POLLHUP)
				{
					// Connection closed
					consolePrint(TEXT("pollserver: socket %1!llu! aborted connection\n"), pfds[i].fd);

					closesocket(pfds[i].fd); // Bye!
					disconnect(pfds[i].fd);
					pfds.erase(pfds.begin() + i);
				}
			} // END looping through file descriptors
		} // END for(;;)--and you thought it would never end!
	}

	void Socket::OnConnect(std::function<void(unsigned __int64&, TCHAR*, Socket&)> funcPtr, unsigned __int64  scktNbr, TCHAR* buf)
	{
		funcPtr(scktNbr, buf, *this);
	}

	TSTR Socket::OnServerReceive(std::function<TSTR(TCHAR*, unsigned __int64&)> funcPtr, 
										 TCHAR* data, unsigned __int64& scktNbr)
	{
		return funcPtr(data, scktNbr);
	}

	void Socket::OnReceiveData(std::function<void(TCHAR*)> funcPtr)
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();
		char buf[256];    // Buffer for client data

		int recvBytes = recv(pfds[0].fd, buf, sizeof buf, 0);
		buf[recvBytes - 1] = '\0';

		if (recvBytes < 0)
		{
			reportWindowsError(TEXT("recv"), WSAGetLastError());
		}
		else
		{
			funcPtr((TCHAR*)buf);
		}

		WSAResetEvent(m_Handle);
	}

	HANDLE Socket::GetHandle()
	{
		return m_Handle;
	}

	Socket::~Socket()
	{

	}

	void Socket::PrintSocketAddr(addrinfo* list)
	{
		struct addrinfo* iter;
		consolePrint(TEXT("IP addresses for socket:\n"));
		for (iter = list; iter != nullptr; iter = iter->ai_next)
		{
			void* addr;
			LPCTSTR ipver;

			// get the pointer to the address itself,
			// different fields in IPv4 and IPv6:
			if (iter->ai_family == AF_INET)
			{ // IPv4
				struct sockaddr_in* ipv4 = (struct sockaddr_in*)iter->ai_addr;
				addr = &(ipv4->sin_addr);
				ipver = TEXT("IPv4");
			}
			else
			{ // IPv6
				struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)iter->ai_addr;
				addr = &(ipv6->sin6_addr);
				ipver = TEXT("IPv6");
			}

			TCHAR ipstr[INET6_ADDRSTRLEN * sizeof(TCHAR)];   /* IP string */

			// convert the IP to a string and print it:
			InetNtop(iter->ai_family, addr, (LPTSTR)ipstr, sizeof ipstr);
			//inet_ntop(iter->ai_family, addr, (PSTR)ipstr, sizeof ipstr);
			consolePrint(TEXT("  %1!s!: %2!s!\n"), ipver, ipstr);
		}
	}
}
