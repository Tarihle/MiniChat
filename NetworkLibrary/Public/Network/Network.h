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

		short NewSocket();
		short NewSocketBind();
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