#pragma once

#include "../pch.h"
#include <functional>

#define MAX_BUF_SIZE 510 /* Network will force-add \0 char as 511th byte */

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
		void Send(LPCTSTR buf, int len);
		void Send(LPCTSTR buf, int len, unsigned __int64 destination);
		void SendAll(LPCTSTR buf, int len);
		void SendAll(LPCTSTR buf, int len, unsigned __int64 unwantedDestination);
		void PollLoop(std::function<void(unsigned __int64&, TCHAR*&, Socket&)> connect, 
					  std::function<TSTR(TCHAR*, unsigned __int64&)> receive,
					  std::function<void(unsigned __int64)> disconnect);
		void OnConnect(std::function<void(unsigned __int64&, TCHAR*&, Socket&)> funcPtr, unsigned __int64 scktNbr, TCHAR*& buf);
		TSTR OnServerReceive(std::function<TSTR(TCHAR*, unsigned __int64&)> funcPtr, 
									 TCHAR* data, unsigned __int64& scktNbr);
		void OnReceiveData(std::function<void(TCHAR*)> funcPtr);
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