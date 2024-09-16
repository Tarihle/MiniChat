#pragma once

namespace Net
{
	class Network
	{
	public:
		/// <summary>Default Constructor.</summary>
		Network();

		//////////////////////////////////////////////////
		
		static Network* GetInstance();

		int Startup();

		int Cleanup();

		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Network();
	private:
		static Network* m_Instance;
	};
}