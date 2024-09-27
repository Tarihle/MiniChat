#pragma once
#include <Network/Network.h>

namespace Chat
{
	class Client
	{
	public:
		Client();
		Client(Client& other) = delete;

		//////////////////////////////////////////////////

		void	Connect(TSTR IP, LPCTSTR username, int len);
		HANDLE	GetSocketHandle();
		void	SetConsole(HANDLE& hConsole, DWORD& oldMode, DWORD& newMode);
		void	InputConsole(HANDLE hConsole, PINPUT_RECORD inRec, DWORD& recRead, DWORD oldMode);
		void	KeyEventProc(KEY_EVENT_RECORD ker);
		void	MouseEventProc(MOUSE_EVENT_RECORD mer);
		void	ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr);
		void	ErrorExit(LPCTSTR lpszMessage, HANDLE hConsole, DWORD oldMode);
		void	SendMsg(LPCTSTR msg, int length);
		void	ReceiveMsg();
		void	Close();
		void	Run(HANDLE& hConsole, DWORD& oldMode);
		
		//////////////////////////////////////////////////

		Client& operator=(Client& rhs) = delete;
		
		//////////////////////////////////////////////////
		~Client();

		bool	m_ShouldClose = false;
	private:
		Net::Socket*	m_Socket = nullptr;
		short*			m_ErrCode = nullptr;

		TSTR	m_CharBuf;
	};

	bool	HandleMsg(TCHAR* msg);

}
