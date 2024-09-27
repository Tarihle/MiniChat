#pragma once
#include <Network/Network.h>

namespace Chat
{
	class Client
	{
	public:
		////////////////////////////////////////////////// CONSTRUCTORS
		/// <summary>Non-parameterized constructor.</summary>
		Client();

		/// <summary>Copy constructor.</summary>
		/// <param name="other">: Server to copy.</param>
		Client(Client& other) = delete;

		////////////////////////////////////////////////// FUNCTIONS
		/// <summary>Creates a client socket and connects it to a server.</summary>
		void	Connect();

		/// <summary>Client's socket event handle getter.</summary>
		/// <returns>Copy of the socket event handle.</returns>
		HANDLE	GetSocketHandle();

		/// <summary>Sets the console to handle input events.</summary>
		/// <param name="hConsole">: Console handle.</param>
		/// <param name="oldMode">: Saved console mode.</param>
		/// <param name="newMode">: New console mode.</param>
		void	SetConsole(HANDLE& hConsole, DWORD& oldMode, DWORD& newMode);

		/// <summary>Handles the reading of console inputs.</summary>
		/// <param name="hConsole">: Console handle.</param>
		/// <param name="inRec">: Array of input events.</param>
		/// <param name="recRead">: Number of input read.</param>
		/// <param name="oldMode">: Saved console mode.</param>
		void	InputConsole(HANDLE hConsole, PINPUT_RECORD inRec, DWORD& recRead, DWORD oldMode);

		/// <summary>Processes keyboard events.</summary>
		/// <param name="ker">: Keyboard Event Record.</param>
		void	KeyEventProc(KEY_EVENT_RECORD ker);

		/// <summary>Handles error codes.</summary>
		/// <param name="lpszMessage"></param>
		/// <param name="hConsole">: Console handle.</param>
		/// <param name="oldMode">: Saved console mode.</param>
		void	ErrorExit(LPCTSTR lpszMessage, HANDLE hConsole, DWORD oldMode);

		/// <summary>Handles sending of messages to the server.</summary>
		/// <param name="msg">: Message to send.</param>
		/// <param name="length">: Length of the message.</param>
		void	SendMsg(LPCTSTR msg, int length);

		/// <summary>Handles reception of messages from the server.</summary>
		void	ReceiveMsg();

		/// <summary>Main loop of the client, handling input and receive from server.</summary>
		/// <param name="hConsole">: Console handle.</param>
		/// <param name="oldMode">: Saved console mode.</param>
		void	Run(HANDLE& hConsole, DWORD& oldMode);
		
		////////////////////////////////////////////////// OPERATORS
		/// <summary>Copy assignment.</summary>
		/// <param name="rhs">: Client to copy data from.</param>
		/// <returns>Modified client.</returns>
		Client& operator=(Client& rhs) = delete;
		
		////////////////////////////////////////////////// DESTRUCTOR
		/// <summary>Destructor.</summary>
		~Client();

		/// <summary>Boolean to determine if the client should close.</summary>
		bool	m_ShouldClose = false;
	private:
		/// <summary>Pointer to client's socket.</summary>
		Net::Socket*	m_Socket = nullptr;

		/// <summary>UNUSED: Short to store error codes.</summary>
		short*			m_ErrCode = nullptr;

		/// <summary>Array of character to write into.</summary>
		TSTR	m_CharBuf;
	};

	/// <summary>Handle printing of messages on the console.</summary>
	/// <param name="msg">: Message to print.</param>
	/// <returns>True if the client should close or the message is invalid, false otherwise.</returns>
	bool	HandleMsg(TCHAR* msg);

}
