#pragma once
#include <Network/Network.h>

namespace Chat
{
	class Client
	{
	public:
		Client();
		Client(Client& other) = delete;

		//////////////////////////////////////////////////

		void Connect();
		
		//////////////////////////////////////////////////

		Client& operator=(Client& rhs) = delete;
		
		//////////////////////////////////////////////////
		~Client();

	private:
		Net::Socket*	m_Socket = nullptr;
		short*			m_ErrCode = nullptr;
	};
}
