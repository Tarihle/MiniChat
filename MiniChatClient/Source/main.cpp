///Client
#include <Network/Network.h>
#include <stdio.h> /* printf */

////////////////////////////////////////////////////////Readconsole

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _UNICODE
#undef _UNICODE
#endif

#ifdef UNICODE
#undef UNICODE
#endif

HANDLE hStdin;
DWORD fdwSaveOldMode;
CHAR charbuf[256];
short bufidx = 0;

VOID ErrorExit(LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker, Net::Socket client)
{
    //printf("Key event: ");

    if (!ker.bKeyDown)
    {
        return;
    }

    //printf("key pressed\n");

    if (ker.wVirtualKeyCode != VK_RETURN && ker.wVirtualKeyCode != VK_ESCAPE)
    {
        charbuf[bufidx] = ker.uChar.AsciiChar;
        bufidx++;
    }
    else if (ker.wVirtualKeyCode == VK_RETURN)
    {
        charbuf[bufidx] = '\0';
        bufidx++;
        //wprintf(wcharbuf);
        client.Send((char*)charbuf, bufidx);
        bufidx = 8;
    }
    else
    {
        client.Close();
    }
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
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

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    printf("Resize event\n");
    printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}

int main()
{
    short* error = new short(-1);
    Net::Socket client(error);

    client.NewSocketConnect("10.5.5.105", "27015", 1); /* IANA says port 6698 is unassigned */

    printf("Connecting to 10.5.5.106\n");

    DWORD cNumRead, fdwMode, i;
    CHAR name[8] = {'L', 'o', 'u', 'i', 's', ' ', '>', ' '};
    INPUT_RECORD irInBuf[128];
    for (int ind = 0; ind < 8; ind++)
    {
        charbuf[ind] = name[ind];
    }
    bufidx = 8;

    // Get the standard input handle.

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit((LPSTR)"GetStdHandle");

    // Save the current input mode, to be restored on exit.

    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit((LPSTR)"GetConsoleMode");

    // Enable the window and mouse input events.

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit((LPSTR)"SetConsoleMode");

    HANDLE eventHandles[2] = { hStdin, client.GetHandle() };

    // Loop to read and handle the next 100 input events.

    while (true)
    {
        DWORD object = WaitForMultipleObjects(2, eventHandles, false, INFINITE);

        // Wait for the events.
        //client.PollClient();
        switch (object)
        {
        case WAIT_OBJECT_0:
            if (!ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead)) // number of records read
                ErrorExit((LPSTR)"ReadConsoleInput");

            // Dispatch the events to the appropriate handler.

            for (i = 0; i < cNumRead; i++)
            {
                switch (irInBuf[i].EventType)
                {
                case KEY_EVENT: // keyboard input
                    KeyEventProc(irInBuf[i].Event.KeyEvent, client);
                    break;

                case MOUSE_EVENT: // mouse input
                    MouseEventProc(irInBuf[i].Event.MouseEvent);
                    break;

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
                    break;

                case FOCUS_EVENT:  // disregard focus events

                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    ErrorExit((LPSTR)"Unknown event type");
                    break;
                }
            }            break;
        case WAIT_OBJECT_0 + 1:
            //printf("Socket\n");
            client.PollClient();
            break;
        default:
            break;
        }


    }

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}