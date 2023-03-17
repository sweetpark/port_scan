#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;

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

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }
    printf("Bind done.\n");

    listen(s, 3);

    printf("Waiting for incoming connections...\n");

    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
    {
        char *message = "Hello Client, I have received your connection.\n";
        send(new_socket, message, strlen(message), 0);
    }

    if (new_socket == INVALID_SOCKET)
    {
        printf("Accept failed with error code : %d", WSAGetLastError());
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
