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
		std::vector<pollfd>& GetPollfds();
		void AddPollfd(SOCKET& newSocket);

		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Network();
	private:
		static Network* m_Instance;
		std::vector<pollfd> m_Pollfds;
	};
}