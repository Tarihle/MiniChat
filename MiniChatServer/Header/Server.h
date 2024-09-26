#pragma once
#include <Network/Network.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unordered_map>

namespace Chat
{
	static std::unordered_map<unsigned __int64, std::string>	Usernames;

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
		Net::Socket*	m_Listener = nullptr;
		short*			m_ErrCode = nullptr;
	};

	void HandleConnection(unsigned __int64& socket, char* username, Net::Socket& server);
	std::string HandleData(char* data, unsigned __int64& socket);
	void HandleDisconnect(unsigned __int64 socket);

}