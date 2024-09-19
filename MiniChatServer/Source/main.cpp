///Server
#include <Network/Network.h>

using namespace Net;

int main()
{
    short* error = new short(-1);
    Net::Socket server(error);

    server.NewSocketBind("10.5.5.106", "6698", 1); /* IANA says port 6698 is unassigned */

    server.Listening();
    //server.Accepting();
    server.PollLoop();

    return 0;
}
