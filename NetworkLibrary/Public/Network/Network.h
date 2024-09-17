#pragma once

namespace Net
{
	class Socket
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Socket() = delete;
		Socket(short*& errorOutput);

		//////////////////////////////////////////////////

		short NewSocket(short optionalPrint = 0);
		short NewSocketBind(short optionalPrint = 0);
		void Listening();
		void Accepting();
		 
		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Socket();

	private:
		void* m_Network = nullptr;
		short m_ID = -1;
	};
}