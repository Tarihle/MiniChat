#include "Client.h"

namespace Chat
{
	Client::Client()
	{
		m_ErrCode = new short(-1);

		m_Socket = new Net::Socket(m_ErrCode);
	}

	void Client::Connect()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->NewSocketConnect("10.5.5.105", "27015", 1);
	}

	void Client::GetSocketHandle()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->GetHandle();
	}

	Client::~Client()
	{
		delete m_ErrCode;
		delete m_Socket;
	}
}
