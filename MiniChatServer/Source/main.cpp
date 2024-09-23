///Server
#include "Server.h"

int main()
{
    //short* error = new short(-1);
    //Net::Socket server(error);

    //server.NewSocketBind("10.5.5.106", "6698", 1); /* IANA says port 6698 is unassigned */

    //server.Listening();
    ////server.Accepting();
    //server.PollLoop();
    Chat::Server server;

    server.CreateServer();

    server.Run();

    //HANDLE eventHandles[2] = { server.GetListenerHandle(), server.GetReceiverHandle() };

    //while (true)
    //{
    //    DWORD eventSocket = WaitForMultipleObjects(2, eventHandles, false, INFINITE);

    //    switch (eventSocket)
    //    {
    //    case WAIT_OBJECT_0:
    //        break;
    //    case WAIT_OBJECT_0 + 1:
    //        break;
    //    default:
    //        break;
    //    }
    //}

    return 0;
}
