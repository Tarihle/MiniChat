#pragma once
#include <Network/Network.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unordered_map>
#include <string>

namespace Chat
{
	class Server
	{
	public:
		Server();
		Server(Server& other) = delete;

		//////////////////////////////////////////////////

		void	CreateServer();
		void	Run();
		HANDLE	GetListenerHandle();

		//////////////////////////////////////////////////

		Server& operator=(Server& rhs) = delete;

		//////////////////////////////////////////////////
		~Server();

	private:
		std::unordered_map<unsigned __int64, std::string>	m_Usernames;

		Net::Socket*	m_Listener = nullptr;

		short*	m_ErrCode = nullptr;
	};

	void HandleConnection(unsigned __int64& socket, Net::Socket& server);

}