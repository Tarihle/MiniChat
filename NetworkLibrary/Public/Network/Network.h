#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN	/* No need for every windows header */
#include <Windows.h>
#endif

namespace Net
{
	class Socket
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Socket() = delete;
		Socket(short*& errorOutput);

		//////////////////////////////////////////////////

		short NewSocket(const char* IPAddress, const char* port, short optionalPrint = 0);
		short NewSocketBind(const char* IPAddress, const char* port, short optionalPrint = 0);
		short NewSocketConnect(const char* IPAddress, const char* port, short optionalPrint = 0);
		void* GetAddr(struct sockaddr* SocketAddress);
		void Listening();
		void Accepting();
		void Close();
		void Send(const char* buf, int len);
		void PollLoop();
		void PollClient();
		HANDLE GetHandle();
		 
		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Socket();

	private:
		void PrintSocketAddr(struct addrinfo* list);

		//////////////////////////////////////////////////

		void* m_Network = nullptr;
		short m_ID = -1;
		HANDLE m_Handle = nullptr;
	};
}