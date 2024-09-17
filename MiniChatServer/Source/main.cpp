///Server
#include <Network/Network.h>

using namespace Net;

int main()
{
    short* error = new short(-1);
    Net::Socket server(error);

    server.NewSocketBind("localhost", "6698", 1); /* IANA says port 6698 is unassigned */

    server.Listening();
    server.Accepting();

    return 0;
}
