#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // include the Windows socket library

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    int recv_size;
    int retval;

    // initialize Winsock
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    
    // create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    // set the socket to non-blocking mode
    
    u_long mode = 1; // 1 to enable non-blocking mode, 0 to disable
    retval = ioctlsocket(sock,FIONBIO, &mode);
    if (retval != 0) {
        printf("ioctlsocket failed with error : %d", WSAGetLastError());
    }

    // prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr("192.168.168.128"); // set the IP address of the server
    server.sin_family = AF_INET;
    server.sin_port = htons(22); // set the port number of the server

    // connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect error");
        return 1;
    }
    
    printf("Connected\n");

    // send some data
    strcpy(message, "Hello Server\n");
    if (send(sock, message, strlen(message), 0) < 0) {
        printf("Send failed");
        return 1;
    }

    // wait for a response using the select model
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    timeout.tv_sec = 5; // set the timeout to 5 seconds
    timeout.tv_usec = 0;
    int select_result = select(0, &readfds, NULL, NULL, &timeout);
    if (select_result == SOCKET_ERROR) {
        printf("Select failed with error : %d", WSAGetLastError());
    } else if (select_result == 0) {
        printf("Timeout occurred\n");
    } else {
        // receive the server's response
        if ((recv_size = recv(sock, server_reply, 2000, 0)) == SOCKET_ERROR) {
            printf("recv failed with error : %d", WSAGetLastError());
        } else {
            // add null character to the end of the string
            server_reply[recv_size] = '\0';
            printf("Server reply : %s\n", server_reply);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
