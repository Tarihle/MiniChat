#pragma once
#include <Network/Network.h>

#include <unordered_map>

namespace Chat
{
	/// <summary>Unordered map of clients' username, using their socket as key.</summary>
	static std::unordered_map<unsigned __int64, TSTR>	Usernames;

	class Server
	{
	public:
		////////////////////////////////////////////////// CONSTRUCTORS
		/// <summary>Non-parameterized constructor.</summary>
		Server();

		/// <summary>Copy constructor.</summary>
		/// <param name="other">: Server to copy.</param>
		Server(Server& other) = delete;

		////////////////////////////////////////////////// FUNCTIONS
		/// <summary>Creates server socket, binds it and listen to it.</summary>
		void	CreateServer();

		/// <summary>Main loop of the server, based on WSAPoll</summary>
		void	Run();

		/// <summary>Listener event handle getter.</summary>
		/// <returns>Copy of the listener event handle.</returns>
		HANDLE	GetListenerHandle();

		////////////////////////////////////////////////// OPERATORS
		/// <summary>Copy assignment.</summary>
		/// <param name="rhs">: Server to copy data from.</param>
		/// <returns>Modified server.</returns>
		Server& operator=(Server& rhs) = delete;

		////////////////////////////////////////////////// DESTRUCTOR
		/// <summary>Destructor.</summary>
		~Server();

	private:
		/// <summary>Pointer to server's socket.</summary>
		Net::Socket*	m_Listener = nullptr;

		/// <summary>UNUSED: Short to store error codes.</summary>
		short*			m_ErrCode = nullptr;
	};

	/// <summary>Handles the connection of a client.</summary>
	/// <param name="socket">: Client's socket.</param>
	/// <param name="username">: Client's username.</param>
	/// <param name="server">: Server's socket.</param>
	void HandleConnection(unsigned __int64& socket, TCHAR*& username, Net::Socket& server);
	
	/// <summary>Handles the data sent by a client.</summary>
	/// <param name="data">: Data sent.</param>
	/// <param name="socket">: Client's socket.</param>
	/// <returns>Data prepended with the client's username.</returns>
	TSTR HandleData(TCHAR* data, unsigned __int64& socket);
	
	/// <summary>Handles the disconnection of a client.</summary>
	/// <param name="socket">: Client's socket.</param>
	void HandleDisconnect(unsigned __int64 socket);

}