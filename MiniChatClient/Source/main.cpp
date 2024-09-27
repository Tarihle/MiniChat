///Client
#include "Client.h"
#include <iostream> /* cin */

int appMain()
{
    HANDLE hStdin;
    DWORD fdwSaveOldMode;

    Chat::Client client;

    TSTR username;
    TSTR IP;
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

    client.Connect(IP, (LPCTSTR)username.c_str(), (int)username.size());
    client.Run(hStdin, fdwSaveOldMode);

    return 0;
}

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int _tmain()
{
	_CrtMemState start;
	_CrtMemCheckpoint(&start);

	int returnValue = appMain();

	_CrtMemState end;
	_CrtMemCheckpoint(&end);

	_CrtMemState difference;
	if (_CrtMemDifference(&difference, &start, &end))
	{
		OutputDebugString(TEXT("---------- _CrtMemDumpStatistics ----------\n\n"));
		_CrtMemDumpStatistics(&difference);
		OutputDebugString(TEXT("\n---------- _CrtMemDumpAllObjectsSince ----------\n\n"));
		_CrtMemDumpAllObjectsSince(&end);
		OutputDebugString(TEXT("\n---------- _CrtMemDumpMemoryLeaks ----------\n\n"));
		_CrtDumpMemoryLeaks();

		if (!returnValue)
		{
			return -1;
		}
	}

	return returnValue;
}
