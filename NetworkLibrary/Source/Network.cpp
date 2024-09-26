#include "PrivateNetwork.h"
#include "ErrorHandling.hpp"

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

		connect(((Network*)m_Network)->GetPollfds()[idx].fd, list->ai_addr, (int)list->ai_addrlen);

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
			consolePrint("New client connected\n");
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

	void Socket::Send(const char* buf, int len)
	{
		//consolePrint("%1!s!\n", buf);
		send(((Network*)m_Network)->GetPollfds()[0].fd, buf, len + 1, 0); /* +1 because we will force \0 on the last character */
	}

	void Socket::Send(const char* buf, int len, unsigned __int64 destination)
	{
		consolePrint("%1!s!\n", buf);
		send(destination, buf, len + 1, 0); /* +1 because we will force \0 on the last character */
	}

	void Socket::SendAll(const char* buf, int len)
	{
		std::vector<pollfd>& pfds = ((Network*)m_Network)->GetPollfds();

		// We got some good data from a client
		for (int j = 1; j < pfds.size(); j++) /* Beginning at 1 to avoid sending to listener */
		{
			// Send to everyone!
			SOCKET destination = pfds[j].fd;

			if (SOCKET_ERROR == send(destination, buf, len + 1, 0)) /* +1 because we will force \0 on the last character */
			{
				reportWindowsError(TEXT("SendAll"), WSAGetLastError());
			}
		}
	}

	void Socket::SendAll(const char* buf, int len, unsigned __int64 unwantedDestination)
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
				if (SOCKET_ERROR == send(destination, buf, len + 1, 0)) /* +1 because we will force \0 on the last character */
				{
					reportWindowsError(TEXT("sendAll"), WSAGetLastError());
				}
			}
		}
	}

	void Socket::PollLoop(std::function<void(unsigned __int64&, char*, Socket&)> connect, 
						  std::function<std::string(char*, unsigned __int64&)> receive)
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

							consolePrint("pollserver: new connection from %1!s! on socket %2!llu!\n",
								inet_ntop(remoteAddr.ss_family, GetAddr((struct sockaddr*)&remoteAddr), remoteIP, INET6_ADDRSTRLEN), 
								newFd);
							consolePrint("Numbers of sockets check: %1!d!\n", (int)pfds.size());

							recv(pfds[pfds.size() - 1].fd, buf, sizeof buf, 0);

							OnConnect(connect, newFd, buf);
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
							std::string treatedData = OnServerReceive(receive, buf, sender);

							consolePrint(TEXT("%1!s!\n"), treatedData.c_str());

							// We got some good data from a client
							for (int j = 0; j < pfds.size(); j++) 
							{
								// Send to everyone!
								SOCKET destination = pfds[j].fd;

								// Except the serverListener and ourselves
								if (destination != serverListener && destination != sender) 
								{
									if (SOCKET_ERROR == send(destination, treatedData.c_str(), (int)treatedData.size() + 1, 0))
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
					pfds.erase(pfds.begin() + i);
				}
			} // END looping through file descriptors
		} // END for(;;)--and you thought it would never end!
	}

	void Socket::OnConnect(std::function<void(unsigned __int64&, char*, Socket&)> funcPtr, unsigned __int64  scktNbr, char* buf)
	{
		funcPtr(scktNbr, buf, *this);
	}

	std::string Socket::OnServerReceive(std::function<std::string(char*, unsigned __int64&)> funcPtr, 
										 char* data, unsigned __int64& scktNbr)
	{
		return funcPtr(data, scktNbr);
	}

	void Socket::OnReceiveData(std::function<void(char*)> funcPtr)
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
			funcPtr(buf);
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
