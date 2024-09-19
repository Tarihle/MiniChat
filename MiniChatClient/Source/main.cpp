///Client
#include <Network/Network.h>
#include <stdio.h> /* printf */

int main()
{
    short* error = new short(-1);
    Net::Socket client(error);

    client.NewSocketConnect("10.5.5.106", "6698", 1); /* IANA says port 6698 is unassigned */

    printf("Connecting to 10.5.5.106\n");

    //char buf[100];
    //int charCount = recv(sckt, buf, 100, 0);
    //buf[charCount-1] = '\0';

    //_tprintf(TEXT("%s\n"), buf);

    //std::ofstream chatLogs;
    //chatLogs.open("ChatLogs.txt");
    //chatLogs << buf;
    //chatLogs.close();

    //char msg[7] = "Ye boi";
    //int len, bytes_sent;

    //len = (int)strlen(msg) + 1;
    //bytes_sent = send(sckt, msg, len, 0);

    return 0;
}