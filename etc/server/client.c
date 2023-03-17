#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    int recv_size;

    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connect error.\n");
        return 1;
    }

    printf("Connected.\n");

    if (recv_size = recv(s, server_reply, 2000, 0) > 0)
    {
        printf("%s", server_reply);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
