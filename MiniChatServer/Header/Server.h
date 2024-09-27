#pragma once
#include <Network/Network.h>

#include <unordered_map>

namespace Chat
{
	static std::unordered_map<unsigned __int64, TSTR>	Usernames;

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

	void HandleConnection(unsigned __int64& socket, TCHAR*& username, Net::Socket& server);
	TSTR HandleData(TCHAR* data, unsigned __int64& socket);
	void HandleDisconnect(unsigned __int64 socket);

}