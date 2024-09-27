#include "Client.h"
#include <iostream> /* cin */

namespace Chat
{
	Client::Client()
	{
		m_ErrCode = new short(-1);

		m_Socket = new Net::Socket(m_ErrCode);
	}

	void Client::Connect()
	{
		if (nullptr == m_Socket)
		{
			return;
		}
		
		bool validConnection = false;
		TSTR username;
		TSTR IP;

		while (!validConnection)
		{
#ifdef UNICODE
			std::wcout << L"Choose a username: ";
			std::wcin >> username;
			std::wcout << L"Enter IP address: ";
			std::wcin >> IP;
#else
			std::cout << "Choose a username: ";
			std::cin >> username;
			std::cout << "Enter IP address: ";
			std::cin >> IP;
#endif

#ifdef UNICODE
			std::string IPstring;
			for (int i = 0; i < IP.size(); i++)
			{
				IPstring.push_back((char)IP[i]);
			}

			validConnection = m_Socket->NewSocketConnect(IPstring.c_str(), "6698", 1); /* Louis */
#else
			validConnection = m_Socket->NewSocketConnect(IP.c_str(), "6698", 1); /* Louis */
#endif
			}

		m_Socket->Send(username.c_str(), (int)username.size());
	}

	HANDLE Client::GetSocketHandle()
	{
		if (nullptr == m_Socket)
		{
			return nullptr;
		}

		return m_Socket->GetHandle();
	}

	void Client::SetConsole(HANDLE& hConsole, DWORD& oldMode, DWORD& newMode)
	{
		hConsole = GetStdHandle(STD_INPUT_HANDLE);

		// Get the standard input handle.
		if (hConsole == INVALID_HANDLE_VALUE)
			ErrorExit(TEXT("GetStdHandle"), hConsole, oldMode);

		// Save the current input mode, to be restored on exit.
		if (!GetConsoleMode(hConsole, &oldMode))
			ErrorExit(TEXT("GetConsoleMode"), hConsole, oldMode);

		// Enable the window and mouse input events.
		newMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(hConsole, newMode))
			ErrorExit(TEXT("SetConsoleMode"), hConsole, oldMode);
	}

	void Client::InputConsole(HANDLE hConsole, PINPUT_RECORD inRec, DWORD& recRead, DWORD oldMode)
	{
		if (!ReadConsoleInput(
			hConsole,		/* input buffer handle */
			inRec,			/* buffer to read into */
			MAX_BUF_SIZE,	/* size of read buffer */
			&recRead))		/* number of records read */
			ErrorExit(TEXT("ReadConsoleInput"), hConsole, oldMode);

		/* Dispatch the events to the appropriate handler. */
		for (DWORD i = 0; i < recRead; i++)
		{
			switch (inRec[i].EventType)
			{
			case KEY_EVENT: /* keyboard input */
				KeyEventProc(inRec[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: /* disregard mouse input */

			case WINDOW_BUFFER_SIZE_EVENT: /* disregard scrn buf. resizing */

			case FOCUS_EVENT:  /* disregard focus events */

			case MENU_EVENT:   /* disregard menu events */
				break;

			default:
				ErrorExit(TEXT("Unknown event type"), hConsole, oldMode);
				break;
			}
		}
	}
	
	void Client::KeyEventProc(KEY_EVENT_RECORD ker)
	{
		if (!ker.bKeyDown || VK_SHIFT == ker.wVirtualKeyCode)
		{
			return;
		}

		if (VK_RETURN != ker.wVirtualKeyCode && 
			VK_ESCAPE != ker.wVirtualKeyCode && 
			VK_BACK != ker.wVirtualKeyCode && 
			VK_DOWN != ker.wVirtualKeyCode)
		{
			m_CharBuf.push_back((TCHAR)ker.uChar.UnicodeChar);
			_tprintf(TEXT("%c"), (TCHAR)ker.uChar.UnicodeChar);
		}
		else if (VK_RETURN == ker.wVirtualKeyCode)
		{
			_tprintf(TEXT("%c[E"), 27);
			SendMsg(m_CharBuf.c_str(), (int)m_CharBuf.size());
			m_CharBuf.clear();
		}
		else if (VK_BACK == ker.wVirtualKeyCode)
		{
			_tprintf(TEXT("\b \b"));
			if (!m_CharBuf.empty())
				m_CharBuf.pop_back();
		}
	}

	void Client::ErrorExit(LPCTSTR lpszMessage, HANDLE hConsole, DWORD oldMode)
	{
		_tprintf(TEXT("%s\n"), lpszMessage);

		// Restore input mode on exit.
		SetConsoleMode(hConsole, oldMode);

		ExitProcess(0);
	}

	void Client::SendMsg(LPCTSTR msg, int length)
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		TSTR quit = TEXT("/quit\0");

		int idx = 0;
		while (msg[idx] != '\0' && quit[idx] != '\0')
		{
			m_ShouldClose = true;
			if (msg[idx] != quit[idx])
			{
				m_ShouldClose = false;
				break;
			}

			idx += sizeof(TCHAR);
		}

		if (!m_ShouldClose)
		{
			m_Socket->Send(msg, length);
		}
	}

	void Client::ReceiveMsg()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_ShouldClose = m_Socket->OnReceiveData(HandleMsg);
		_tprintf(TEXT("%c[E%s"), 27, m_CharBuf.c_str());
	}

	void Client::Run(HANDLE& hConsole, DWORD& oldMode)
	{
		INPUT_RECORD irInBuf[MAX_BUF_SIZE];
		DWORD cNumRead, fdwMode;

		SetConsole(hConsole, oldMode, fdwMode);
		HANDLE eventHandles[] = { hConsole, GetSocketHandle() };

		/* Loop to read and handle the next 100 input events. */
		while (!m_ShouldClose)
		{
			DWORD object = WaitForMultipleObjects(ARRAYSIZE(eventHandles), eventHandles, false, INFINITE);

			/* Wait for the events. */
			switch (object)
			{
			case WAIT_OBJECT_0:
				InputConsole(hConsole, irInBuf, cNumRead, oldMode);
				break;
			case WAIT_OBJECT_0 + 1:
				ReceiveMsg();
				break;
			default:
				break;
			}
		}

		/* Restore input mode on exit. */
		SetConsoleMode(hConsole, oldMode);
	}

	Client::~Client()
	{
		delete m_ErrCode;
		delete m_Socket;
	}

	bool HandleMsg(TCHAR* msg)
	{
		/* nullptr means the client should close */
		if (nullptr == msg)
		{
			return true;
		}

		_tprintf(TEXT("%c[2K"), 27);
		_tprintf(TEXT("%s"), msg);

		return false;
	}
}
