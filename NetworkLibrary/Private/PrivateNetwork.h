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

		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Network();
	private:
		static Network* m_Instance;
		std::vector<SOCKET> m_Sockets;
		short m_LastGivenID = -1;
	};
}