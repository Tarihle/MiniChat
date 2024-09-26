///Client
#include "Client.h"
#include <stdio.h> /* printf */
#include <string>
#include <iostream> /* cin */

////////////////////////////////////////////////////////

int main()
{
    HANDLE hStdin;
    DWORD fdwSaveOldMode;

    Chat::Client client;

    TSTR username;
#ifdef UNICODE
    std::wcout << L"Choose a username: ";
    std::wcin >> username;
#else
    std::cout << "Choose a username: ";
    std::cin >> username;
#endif
    client.Connect((LPCTSTR)username.c_str(), (int)username.size());

    INPUT_RECORD irInBuf[128];
    DWORD cNumRead, fdwMode;

    client.SetConsole(hStdin, fdwSaveOldMode, fdwMode);

    HANDLE eventHandles[] = { hStdin, client.GetSocketHandle() };

    // Loop to read and handle the next 100 input events.

    while (true)
    {
        DWORD object = WaitForMultipleObjects(ARRAYSIZE(eventHandles), eventHandles, false, INFINITE);

        // Wait for the events.
        switch (object)
        {
        case WAIT_OBJECT_0:
            client.InputConsole(hStdin, irInBuf, cNumRead, fdwSaveOldMode);          
            break;
        case WAIT_OBJECT_0 + 1:
            client.ReceiveMsg();
            break;
        default:
            break;
        }
    }

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}
