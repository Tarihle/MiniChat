#include "Server.h"

namespace Chat
{
	Server::Server()
	{
		m_ErrCode = new short(-1);

		m_Listener = new Net::Socket(m_ErrCode);
		m_Receiver = new Net::Socket(m_ErrCode);
	}

	Server::~Server()
	{
		delete m_Receiver;
		delete m_Listener;
		delete m_ErrCode;
	}
}
