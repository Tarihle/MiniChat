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

		//m_Socket->NewSocketConnect("10.5.5.105", "27015", 1); /* Malo */
		//m_Socket->NewSocketConnect("10.5.5.108", "8080", 1); /* VinKé */
		m_Socket->NewSocketConnect("10.5.5.106", "6698", 1); /* VinKé */
	}

	HANDLE Client::GetSocketHandle()
	{
		if (nullptr == m_Socket)
		{
			return nullptr;
		}

		return m_Socket->GetHandle();
	}

	void Client::SendMsg(char* msg, short length)
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->Send(msg, length);
	}

	void Client::ReceiveMsg()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->RecvClient();
	}

	void Client::Close()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->Close();
	}

	Client::~Client()
	{
		this->Close();

		delete m_ErrCode;
		delete m_Socket;
	}
}
