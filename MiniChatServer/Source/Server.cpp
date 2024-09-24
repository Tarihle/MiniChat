#include "Server.h"

namespace Chat
{
	Server::Server()
	{
		m_ErrCode = new short(-1);

		m_Listener = new Net::Socket(m_ErrCode);
	}

	void Server::CreateServer()
	{
		m_Listener->NewSocketBind("10.5.5.106", "6698", 1);
		m_Listener->Listening();
	}

	void Server::Run()
	{
		m_Listener->PollLoop(HandleConnection);
	}

	HANDLE Server::GetListenerHandle()
	{
		if (nullptr == m_Listener)
		{
			return nullptr;
		}

		return m_Listener->GetHandle();
	}

	Server::~Server()
	{
		delete m_Listener;
		delete m_ErrCode;
	}

	void HandleConnection(unsigned __int64& socket, Net::Socket& server)
	{
		server.Send("Bonjour", 8, socket);
	}
}
