#pragma once
#include <Network/Network.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Chat
{
	class Client
	{
	public:
		Client();
		Client(Client& other) = delete;

		//////////////////////////////////////////////////

		void	Connect();
		HANDLE	GetSocketHandle();
		void	SendMsg(char* msg, short length);
		void	ReceiveMsg();
		void	Close();
		
		//////////////////////////////////////////////////

		Client& operator=(Client& rhs) = delete;
		
		//////////////////////////////////////////////////
		~Client();

	private:
		Net::Socket*	m_Socket = nullptr;
		short*			m_ErrCode = nullptr;
	};
}
