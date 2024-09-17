#pragma once

#include <vector>
#include <WS2tcpip.h>

#include "Network.h"

namespace Net
{
	class Network
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Network() = delete;

		Network(short*& errorOutput);

		//////////////////////////////////////////////////
		
		static Network* GetInstance(short*& errorOutput);
		std::vector<SOCKET>& GetSockets();
		short AddSocket(SOCKET& newSocket);
		SOCKET& GetListener();
		void SetListener(SOCKET& newListener);
		std::vector<pollfd>& GetPollfds();
		void AddPollfd(SOCKET& newSocket);

		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Network();
	private:
		static Network* m_Instance;
		std::vector<SOCKET> m_Sockets;
		std::vector<pollfd> m_Pollfds;
		short m_LastGivenID = -1;
		SOCKET m_Listener;
	};
}