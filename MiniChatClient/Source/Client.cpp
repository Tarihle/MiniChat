#include "Client.h"

namespace Chat
{
	Client::Client()
	{
		m_ErrCode = new short(-1);

		m_Socket = new Net::Socket(m_ErrCode);
	}

	void Client::Connect(const char* username, int len)
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		//m_Socket->NewSocketConnect("10.5.5.105", "27015", 1); /* Malo */
		//m_Socket->NewSocketConnect("10.5.5.108", "8080", 1); /* VinKé */
		m_Socket->NewSocketConnect("10.5.5.106", "6698", 1); /* Louis */

		m_Socket->Send(username, len);
	}

	HANDLE Client::GetSocketHandle()
	{
		if (nullptr == m_Socket)
		{
			return nullptr;
		}

		return m_Socket->GetHandle();
	}

	void Client::InputConsole(HANDLE hConsole, PINPUT_RECORD inRec, DWORD& recRead, DWORD oldMode)
	{
		if (!ReadConsoleInput(
			hConsole,      // input buffer handle
			inRec,     // buffer to read into
			128,         // size of read buffer
			&recRead)) // number of records read
			ErrorExit((LPSTR)"ReadConsoleInput", hConsole, oldMode);

		// Dispatch the events to the appropriate handler.

		for (DWORD i = 0; i < recRead; i++)
		{
			switch (inRec[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				KeyEventProc(inRec[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input
				MouseEventProc(inRec[i].Event.MouseEvent);
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
				ResizeEventProc(inRec[i].Event.WindowBufferSizeEvent);
				break;

			case FOCUS_EVENT:  // disregard focus events

			case MENU_EVENT:   // disregard menu events
				break;

			default:
				ErrorExit((LPSTR)"Unknown event type", hConsole, oldMode);
				break;
			}
		}
	}
	
	void Client::KeyEventProc(KEY_EVENT_RECORD ker)
	{
		if (!ker.bKeyDown)
		{
			return;
		}

		if (VK_RETURN != ker.wVirtualKeyCode && VK_ESCAPE != ker.wVirtualKeyCode && VK_BACK != ker.wVirtualKeyCode && VK_DOWN != ker.wVirtualKeyCode)
		{
			m_CharBuf[m_BufIdx] = ker.uChar.AsciiChar;
			m_BufIdx++;
			printf("%c", ker.uChar.AsciiChar);
		}
		else if (VK_RETURN == ker.wVirtualKeyCode)
		{
			printf("%c[E", 27);
			m_CharBuf[m_BufIdx] = '\0';
			m_BufIdx++;
			SendMsg((char*)m_CharBuf, m_BufIdx);
			m_BufIdx = 0;
		}
		else if (VK_BACK == ker.wVirtualKeyCode)
		{
			printf("\b \b");
			m_BufIdx--;
		}
		else if (VK_DOWN == ker.wVirtualKeyCode)
		{
			printf("%c[2K%c[E", 27, 27);
			printf(m_CharBuf);
		}
	}

	void Client::MouseEventProc(MOUSE_EVENT_RECORD mer)
	{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
		printf("Mouse event: ");

		switch (mer.dwEventFlags)
		{
		case 0:

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				printf("left button press \n");
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				printf("right button press \n");
			}
			else
			{
				printf("button press\n");
			}
			break;
		case DOUBLE_CLICK:
			printf("double click\n");
			break;
		case MOUSE_HWHEELED:
			printf("horizontal mouse wheel\n");
			break;
		case MOUSE_MOVED:
			printf("mouse moved\n");
			break;
		case MOUSE_WHEELED:
			printf("vertical mouse wheel\n");
			break;
		default:
			printf("unknown\n");
			break;
		}
	}

	void Client::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
	{
		printf("Resize event\n");
		printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	}

	void Client::ErrorExit(LPSTR lpszMessage, HANDLE hConsole, DWORD oldMode)
	{
		fprintf(stderr, "%s\n", lpszMessage);

		// Restore input mode on exit.

		SetConsoleMode(hConsole, oldMode);

		ExitProcess(0);
	}

	void Client::SendMsg(char* msg, short length)
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->Send(msg, length);
	}

	void Client::ReceiveMsg()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->OnReceiveData(HandleMsg);
		printf("%c[E%s", 27, m_CharBuf);
	}

	void Client::Close()
	{
		if (nullptr == m_Socket)
		{
			return;
		}

		m_Socket->Close();
	}

	Client::~Client()
	{
		this->Close();

		delete m_ErrCode;
		delete m_Socket;
	}

	void HandleMsg(char* msg)
	{
		printf("%c[2K", 27);
		printf("%s", msg);
	}
}
