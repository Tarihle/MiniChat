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

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        client.ErrorExit((LPSTR)"GetConsoleMode", hStdin, fdwSaveOldMode);

    // Get the standard input handle.
    if (hStdin == INVALID_HANDLE_VALUE)
        client.ErrorExit((LPSTR)"GetStdHandle", hStdin, fdwSaveOldMode);

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
            //if (!ReadConsoleInput(
            //    hStdin,      // input buffer handle
            //    irInBuf,     // buffer to read into
            //    128,         // size of read buffer
            //    &cNumRead)) // number of records read
            //    ErrorExit((LPSTR)"ReadConsoleInput");

            //// Dispatch the events to the appropriate handler.

            //for (i = 0; i < cNumRead; i++)
            //{
            //    switch (irInBuf[i].EventType)
            //    {
            //    case KEY_EVENT: // keyboard input
            //        KeyEventProc(irInBuf[i].Event.KeyEvent, client);
            //        break;

            //    case MOUSE_EVENT: // mouse input
            //        MouseEventProc(irInBuf[i].Event.MouseEvent);
            //        break;

            //    case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
            //        ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
            //        break;

            //    case FOCUS_EVENT:  // disregard focus events

            //    case MENU_EVENT:   // disregard menu events
            //        break;

            //    default:
            //        ErrorExit((LPSTR)"Unknown event type");
            //        break;
            //    }
            //}            
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