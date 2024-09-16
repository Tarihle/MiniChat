#pragma once

#include <vector>

#include "Network.h"

namespace Net
{
	class Network
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Network() = default;

		//////////////////////////////////////////////////
		
		static Network* GetInstance();

		int Startup();

		int Cleanup();

		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Network();
	private:
		static Network* m_Instance;
		//std::vector<Socket>
	};
}