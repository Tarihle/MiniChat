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

    std::string username;
    std::cout << "Choose a username: ";
    std::cin >> username;
    client.Connect(username.c_str(), (int)username.size());

    printf("Connecting to 10.5.5.106\n");

    INPUT_RECORD irInBuf[128];
    DWORD cNumRead, fdwMode;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    // Get the standard input handle.
    if (hStdin == INVALID_HANDLE_VALUE)
        client.ErrorExit((LPSTR)"GetStdHandle", hStdin, fdwSaveOldMode);

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        client.ErrorExit((LPSTR)"GetConsoleMode", hStdin, fdwSaveOldMode);

    // Enable the window and mouse input events.
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
       client. ErrorExit((LPSTR)"SetConsoleMode", hStdin, fdwSaveOldMode);

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
