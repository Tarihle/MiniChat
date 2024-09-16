#pragma once

namespace Net
{
	class Socket
	{
	public:
		/// <summary>Non-parameterized constructor.</summary>
		Socket() = default;

		//////////////////////////////////////////////////

		int NewSocket();
		 
		//////////////////////////////////////////////////

		/// <summary>Destructor.</summary>
		~Socket();

	private:
		
	};
}