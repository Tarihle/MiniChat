#include "Server.h"

namespace Chat
{
	Server::Server()
	{
		m_ErrCode = new short(-1);

		m_Listener = new Net::Socket(m_ErrCode);
		m_Receiver = new Net::Socket(m_ErrCode);
	}

	void Server::CreateServer()
	{
		m_Listener->NewSocketBind("10.5.5.106", "6698", 1);
		m_Listener->Listening();

		m_Receiver->NewSocketConnect("10.5.5.106", "6698", 1);
		m_Listener->Accepting();
	}

	void Server::Run()
	{
		m_Listener->PollLoop();
	}

	HANDLE Server::GetListenerHandle()
	{
		if (nullptr == m_Listener)
		{
			return nullptr;
		}

		return m_Listener->GetHandle();
	}

	HANDLE Server::GetReceiverHandle()
	{
		if (nullptr == m_Receiver)
		{
			return nullptr;
		}

		return m_Receiver->GetHandle();
	}

	Server::~Server()
	{
		delete m_Receiver;
		delete m_Listener;
		delete m_ErrCode;
	}
}
