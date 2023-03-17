// based window

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
//#include <wait.h>


void *recv_(SOCKET sockfd, char *recvline){
    int n;
    memset(recvline,0,sizeof(recvline));
    n=recv(sockfd,recvline,sizeof(recvline)-1,0);
    if (n>0){
        printf("Connection established!\n");
    }else{
        printf("connection failed!\n");
    }

} //병렬처리
int send_(SOCKET sockfd,struct sockaddr_in servaddr, char *sendline,int n,char *recvline){

    for(int i=n; i<n+30; i++){
        
        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=inet_addr("192.168.168.128");
        servaddr.sin_port=htons(i);
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
            printf("connecting fail\n");
            i=i+1;
            return i;
        }
        printf("playing\n");
        strcpy(sendline, "SYN");
        send(sockfd,sendline,strlen(sendline)+1,0);
        recv_(sockfd,recvline);
    }
    n=n+30;
    return n;


    
}



// char sendline[1000];
// char recvline[1000];

int main(void){
    WSADATA wsa;

    SOCKET sockfd;
    DWORD recvTimeout=3000; //3초

    struct sockaddr_in servaddr, cliaddr;
    char sendline[1000];
    char recvline[1000];
    int n;
    int start_port =19;

    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&recvTimeout, sizeof(recvTimeout)); //socket recv 3초 설정



    if (WSAStartup(MAKEWORD(2,2),&wsa)!=0){
        printf("WSAStartup failed\n");
        return 1;
    }

    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET){
        printf("socket creation failed\n");
        WSACleanup();
        return 1;
    }

    // memset(&servaddr,0,sizeof(servaddr));
    // servaddr.sin_family=AF_INET;
    // servaddr.sin_addr.s_addr=inet_addr("192.168.168.128");
    // servaddr.sin_port=htons(22);

    while(1){
        start_port=send_(sockfd,servaddr,sendline,start_port,recvline);
        printf("%d",start_port);
        //recv()를 받으면 start_port값이 바뀌어야함
        //recv_(sockfd,recvline);
        if (start_port>=1024){
            break;
        }
    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
}