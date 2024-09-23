#pragma once
#include <Network/Network.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Chat
{
	class Server
	{
	public:
		Server();
		Server(Server& other) = delete;

		//////////////////////////////////////////////////



		//////////////////////////////////////////////////

		Server& operator=(Server& rhs) = delete;

		//////////////////////////////////////////////////
		~Server();

	private:
		Net::Socket* m_Listener = nullptr;
		Net::Socket* m_Receiver = nullptr;
		short* m_ErrCode = nullptr;
	};
}