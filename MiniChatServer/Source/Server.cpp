#include "Server.h"

namespace Chat
{
	Server::Server()
	{
		m_ErrCode = new short;
		*m_ErrCode = -1;

		m_Listener = new Net::Socket(m_ErrCode);
	}

	void Server::CreateServer()
	{
		m_Listener->NewSocketBind("6698", 1);
		m_Listener->Listening();
	}

	void Server::Run()
	{
		m_Listener->PollLoop(HandleConnection, HandleData, HandleDisconnect);
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

		Usernames.clear();
	}

	void HandleConnection(unsigned __int64& socket, TCHAR*& username, Net::Socket& server)
	{
		TSTR Welcome = TEXT("Hello ");
		Welcome += username;

		if (Usernames.empty())
		{
			Welcome += TEXT("\n Nobody is in the currently in the server.");
		}
		else
		{
			Welcome += TEXT("\n Users currently connected: ");
			for (auto& it : Usernames)
			{
				Welcome += it.second;
				Welcome += TEXT(", ");
			}

			TSTR WelcomeAll = username;
			WelcomeAll += TEXT(" is now online.");
			server.SendAll(WelcomeAll.c_str(), (int)WelcomeAll.size(), socket);
		}

		server.Send(Welcome.c_str(), (int)Welcome.size(), socket);

		Usernames[socket] = username;
	}

	TSTR HandleData(TCHAR* data, unsigned __int64& socket)
	{
		bool serverClose = false;
		TSTR close = TEXT("/close\0");

		int idx = 0;
		while (data[idx] != '\0' && close[idx] != '\0')
		{
			serverClose = true;
			if (data[idx] != close[idx])
				serverClose = false;

			idx += sizeof(TCHAR);
		}

		if (serverClose)
			return TSTR();

		TSTR msg = Usernames[socket];
		msg.append(TEXT(" > "));
		msg += data;
		return msg;
	}

	void HandleDisconnect(unsigned __int64 socket)
	{
		Usernames.erase(socket);
	}
}
