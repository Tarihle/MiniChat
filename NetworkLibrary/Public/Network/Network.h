#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN	/* No need for every windows header */
#include <Windows.h>
#endif

#include <functional>

namespace Net
{
	class Socket
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Socket() = delete;
		Socket(short*& errorOutput);
		Socket(Socket& other) = delete;

		//////////////////////////////////////////////////

		void NewSocket(const char* IPAddress, const char* port, short optionalPrint = 0);
		void NewSocketBind(const char* IPAddress, const char* port, short optionalPrint = 0);
		void NewSocketConnect(const char* IPAddress, const char* port, short optionalPrint = 0);
		void* GetAddr(struct sockaddr* SocketAddress);
		void Listening();
		void Accepting();
		void Close();
		void Send(const char* buf, int len);
		void PollLoop();
		void OnReceiveData(std::function<void(char*)> funcPtr);
		HANDLE GetHandle();
		 
		//////////////////////////////////////////////////

		Socket& operator=(Socket& rhs) = delete;
		 
		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Socket();

	private:
		void PrintSocketAddr(struct addrinfo* list);

		//////////////////////////////////////////////////

		void* m_Network = nullptr;
		HANDLE m_Handle = nullptr;
	};
}