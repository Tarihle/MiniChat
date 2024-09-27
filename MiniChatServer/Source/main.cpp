///Server
#include "Server.h"

int appMain()
{
    Chat::Server server;

    server.CreateServer();

    server.Run();

    return 0;
}

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int _tmain()
{
#ifndef NDEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
#else
    return appMain();
#endif
}
