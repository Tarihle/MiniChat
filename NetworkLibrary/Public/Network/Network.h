#pragma once

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
		void PollLoop();
		 
		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Socket();

	private:
		void PrintSocketAddr(struct addrinfo* list);

		//////////////////////////////////////////////////

		void* m_Network = nullptr;
		short m_ID = -1;
	};
}