#include "Client.h"

namespace Chat
{
	Client::Client()
	{
		m_ErrCode = new short(-1);

		m_Socket = new Net::Socket(m_ErrCode);
	}

	Client::~Client()
	{
		delete m_ErrCode;
		delete m_Socket;
	}
}
