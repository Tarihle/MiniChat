#pragma once
#include <Network/Network.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _UNICODE
#undef _UNICODE
#endif //!_UNICODE

#ifdef UNICODE
#undef UNICODE
#endif //!UNICODE

#if 1	/* 0 == ANSI && 1 == UNICODE */
#define _UNICODE
#define UNICODE
#endif

namespace Chat
{
	class Client
	{
	public:
		Client();
		Client(Client& other) = delete;

		//////////////////////////////////////////////////

		void	Connect(const char* username, int len);
		HANDLE	GetSocketHandle();
		void	InputConsole(HANDLE hConsole, PINPUT_RECORD inRec, DWORD& recRead, DWORD oldMode);
		void	KeyEventProc(KEY_EVENT_RECORD ker);
		void	MouseEventProc(MOUSE_EVENT_RECORD mer);
		void	ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr);
		void	ErrorExit(LPSTR lpszMessage, HANDLE hConsole, DWORD oldMode);
		void	SendMsg(char* msg, short length);
		void	ReceiveMsg();
		void	Close();
		
		//////////////////////////////////////////////////

		Client& operator=(Client& rhs) = delete;
		
		//////////////////////////////////////////////////
		~Client();

	private:
		Net::Socket*	m_Socket = nullptr;
		short*			m_ErrCode = nullptr;

		CHAR	m_CharBuf[MAX_BUF_SIZE] = {};
		short	m_BufIdx = 0;
	};

	void	HandleMsg(char* msg);

}
