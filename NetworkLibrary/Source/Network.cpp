#include "PrivateNetwork.h"
#include "Network.h"

constexpr short Exit_WSAStartup = 1'01;
constexpr short Exit_WSACleanup = 1'02;

constexpr BYTE minorVersion = 2;
constexpr BYTE majorVersion = 2;

//#define PORT "6698"
//#define IP "localhost"

namespace Net
{
	Network* Network::m_Instance = nullptr;

	Network::Network(short*& errorOutput)
	{
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

	std::vector<SOCKET>& Network::GetSockets()
	{
		return m_Sockets;
	}

	short Network::AddSocket(SOCKET& newSocket)
	{
		m_Sockets.push_back(newSocket);

		return ++m_LastGivenID;

	}

	SOCKET& Network::GetListener()
	{
		return m_Listener;
	}

	void Network::SetListener(SOCKET& newListener)
	{
		m_Listener = newListener;
	}

	std::vector<pollfd>& Network::GetPollfds()
	{
		return m_Pollfds;
	}

	void Network::AddPollfd(SOCKET& newSocket)
	{
		pollfd newPollfd;

		newPollfd.fd = newSocket;
		newPollfd.events = POLLIN;

		m_Pollfds.push_back(newPollfd);
	}

	Network::~Network()
	{
		///* WSA Cleanup */
		int result = WSACleanup();
		if (SOCKET_ERROR == result)
		{
			//reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
			//return Exit_WSACleanup;
		}
	}

	//////////////////////////////////////////////////

	Socket::Socket(short*& errorOutput)
	{
		m_Network = Network::GetInstance(errorOutput);
	}

	short Socket::NewSocket(const char* IPAddress, const char* port,short optionalPrint)
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
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			//reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		freeaddrinfo(list); /* free the linked list */

		m_ID = ((Network*)m_Network)->AddSocket(newSocket);

		return m_ID;
	}

	short Socket::NewSocketBind(const char* IPAddress, const char* port, short optionalPrint)
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
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			//reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		m_ID = ((Network*)m_Network)->AddSocket(newSocket);
		((Network*)m_Network)->SetListener(newSocket);
		printf("Server ID: %d\n", m_ID);
		((Network*)m_Network)->AddPollfd(newSocket);

		if (SOCKET_ERROR == bind(((Network*)m_Network)->GetSockets()[m_ID], list->ai_addr, (int)list->ai_addrlen))
		{
			//reportWindowsError(TEXT("bind"), WSAGetLastError());
			printf("aïe aïe aïe");
		}

		freeaddrinfo(list); /* free the linked list */

		return m_ID;
	}

	short Socket::NewSocketConnect(const char* IPAddress, const char* port, short optionalPrint)
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
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			return 2;
		}

		if (optionalPrint)
		{
			PrintSocketAddr(list);
		}

		newSocket = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
		if (INVALID_SOCKET == newSocket)
		{
			//reportWindowsError(TEXT("socket"), WSAGetLastError());
		}

		m_ID = ((Network*)m_Network)->AddSocket(newSocket);
		printf("Client ID: %d\n", m_ID);
		((Network*)m_Network)->AddPollfd(newSocket);

		connect(((Network*)m_Network)->GetSockets()[m_ID], list->ai_addr, (int)list->ai_addrlen);

		freeaddrinfo(list); /* free the linked list */

		return m_ID;
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
		listen(((Network*)m_Network)->GetSockets()[m_ID], 5);
	}

	void Socket::Accepting()
	{
		struct sockaddr_storage their_addr;
		socklen_t addr_size;
		SOCKET new_fd;

		addr_size = sizeof their_addr;
		new_fd = accept(((Network*)m_Network)->GetSockets()[m_ID], (struct sockaddr*)&their_addr, &addr_size);
		if (INVALID_SOCKET != new_fd)
		{
			printf("New client connected");
		}
		else
		{
			//reportWindowsError(TEXT("accept"), WSAGetLastError());
		}
	}

	void Socket::Close()
	{
		//send(((Network*)m_Network)->GetSockets()[m_ID], "", 0, 0);
		char msg[7] = "Ye boi";
		int len, bytes_sent;

		len = (int)strlen(msg) + 1;
		bytes_sent = send(((Network*)m_Network)->GetSockets()[m_ID], msg, len, 0);
	}

	void Socket::PollLoop()
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();
		SOCKET listener = ((Network*)m_Network)->GetListener();

		struct sockaddr_storage remoteAddr; // Client address
		char remoteIP[INET6_ADDRSTRLEN];

		socklen_t addrLength;
		SOCKET newFd;
		char buf[256];    // Buffer for client data

		for (;;) 
		{
			printf("Numbers of sockets check: %d\n", (int)pfds.size());
			int poll_count = WSAPoll(&pfds[0], (ULONG)pfds.size(), -1);

			if (SOCKET_ERROR == poll_count) 
			{
				//reportWindowsError(TEXT("poll"), WSAGetLastError());
				exit(1);
			}

			// Run through the existing connections looking for data to read
			for (int i = 0; i < pfds.size(); i++) 
			{
				// Check if someone's ready to read
				if (pfds[i].revents & POLLIN) 
				{ // We got one!!
					if (pfds[i].fd == listener) 
					{
						// If listener is ready to read, handle new connection
						addrLength = sizeof remoteAddr;
						newFd = accept(listener, (struct sockaddr*)&remoteAddr, &addrLength);

						if (INVALID_SOCKET == newFd) 
						{
							//reportWindowsError(TEXT("accept"), WSAGetLastError());
						}
						else 
						{
							((Network*)m_Network)->AddPollfd(newFd);

							printf("pollserver: new connection from %s on socket %llu\n",
								inet_ntop(remoteAddr.ss_family, GetAddr((struct sockaddr*)&remoteAddr), remoteIP, INET6_ADDRSTRLEN), 
								newFd);
						}
					}
					else 
					{
						// If not the listener, we're just a regular client
						int recvBytes = recv(pfds[i].fd, buf, sizeof buf, 0);

						SOCKET sender = pfds[i].fd;

						if (recvBytes == 0) 
						{
							// Connection closed
							printf("pollserver: socket %llu hung up\n", sender);

							closesocket(pfds[i].fd); // Bye!

							//del_from_pfds(pfds, i, &pfds.size());
						}
						else if (recvBytes < 0)
						{
							//reportWindowsError(TEXT("recv"), WSAGetLastError());
						}
						else 
						{
							// We got some good data from a client
							for (int j = 0; j < pfds.size(); j++) 
							{
								// Send to everyone!
								SOCKET destination = pfds[j].fd;

								// Except the listener and ourselves
								if (destination != listener && destination != sender) 
								{
									if (SOCKET_ERROR == send(destination, buf, recvBytes, 0)) 
									{
										//reportWindowsError(TEXT("send"), WSAGetLastError());
									}
								}
							}
						}
					} // END handle data from client
				} // END got ready-to-read from poll()
			} // END looping through file descriptors
		} // END for(;;)--and you thought it would never end!
	}

	Socket::~Socket()
	{

	}

	void Socket::PrintSocketAddr(addrinfo* list)
	{
		struct addrinfo* iter;
		printf(TEXT("IP addresses for socket:\n\n"));
		for (iter = list; iter != nullptr; iter = iter->ai_next)
		{
			void* addr;
			const char* ipver;

			// get the pointer to the address itself,
			// different fields in IPv4 and IPv6:
			if (iter->ai_family == AF_INET)
			{ // IPv4
				struct sockaddr_in* ipv4 = (struct sockaddr_in*)iter->ai_addr;
				addr = &(ipv4->sin_addr);
				ipver = "IPv4";
			}
			else
			{ // IPv6
				struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)iter->ai_addr;
				addr = &(ipv6->sin6_addr);
				ipver = "IPv6";
			}

			char ipstr[INET6_ADDRSTRLEN];   /* IP string */

			// convert the IP to a string and print it:
			inet_ntop(iter->ai_family, addr, ipstr, sizeof ipstr);
			printf(TEXT("  %s: %s\n"), ipver, ipstr);
		}
	}
}
