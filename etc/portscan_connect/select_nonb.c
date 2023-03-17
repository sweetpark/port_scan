#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


int main(void){

    WSADATA wsa;
    SOCKET sock;
    SOCKADDR_IN server;
    char server_reply[2000];
    int recv_size;

    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET){
        printf("Could not create socket : %d",WSAGetLastError());
    }
//***********************************************************************************
    u_long mode =1;
        if (ioctlsocket(sock,FIONBIO,&mode) == INVALID_SOCKET){
            printf("ioctlsocket failed with error : %d",WSAGetLastError());
            return 1;
        }
//************************************************************************************
for(int i= 0 ; i < 64; i++)
{
    server.sin_addr.s_addr = inet_addr("192.168.168.128");
    server.sin_family = AF_INET;
    server.sin_port=htons(i);

    connect(~~~);
}
    
    while(TRUE){
        if (connect(sock,(struct sockaddr *)&server,sizeof(server))<0){
            printf("Connect error");
            if(WSAGetLastError() ==WSAEWOULDBLOCK){
                    continue;
                }
                if(WSAGetLastError() == WSAEISCONN){
                    break;
                }
                break;
        }

    }
//*************************************************************************************
    printf("connected\n");


    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sock,&readfds);
    timeout.tv_sec=5; //timeout 3초
    timeout.tv_usec=0;

    int select_result=select(0,&readfds,NULL,NULL,&timeout);
    if (select_result == SOCKET_ERROR){
        printf("Select failed with error : %d", WSAGetLastError());
    }else if (select_result ==0){
        printf("Timeout occurred!\n");
    }else{
        if((recv_size = recv(sock,server_reply,2000,0))==SOCKET_ERROR){
            printf("recv failed with error : %d",WSAGetLastError());
        }else{
            server_reply[recv_size]='\0';
            printf("server reply : %s\n",server_reply);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}