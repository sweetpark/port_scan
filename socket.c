// gcc -o socket socket.c -lwsock32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include<winsock2.h>
#endif

int socket_(char *ip_set,int port){
    #ifdef __linux__
    int sockfd, n;
    #else
    WSADATA wsa;


    SOCKET sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char sendline[1000];
    char recvline[1000];
    int n;
    #endif

    #ifdef __linux__
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket creation failed");
        exit(1);
    }
    #else
    if (WSAStartup(MAKEWORD(2,2),&wsa) !=0 ){
        printf("WSAStartup failed\n");
        return 1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM,0))==INVALID_SOCKET){
        printf("socket creation failed\n");
        WSACleanup();
        return 1;
    }
    #endif
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(ip_set);
    servaddr.sin_port=htons(port);

    //send a syn packet to initiate the connection
    #ifdef __linux__
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        perror("connection failed");
        exit(1);
    }
    strcpy(sendline, "SYN");
    write(sockfd, sendline, strlen(sendline)+1);
    #else
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        printf("connection failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    strcpy(sendline, "SYN");
    send(sockfd, sendline, strlen(sendline)+1, 0);
    //printf("%s\n",sendline);
    #endif

    memset(recvline, 0, sizeof(recvline));
    #ifdef __linux__
    n = read(sockfd, recvline, sizeof(recvline)-1);
    if (n<0){
        perror("read error");
        exit(1);
    }
    if(strcmp(recvline, "SYN-ACK")==0){
        //send an ack packet to complete the connection
        strcpy(sendline,"ACK");
        write(sockfd,sendline,strlen(sendline)+1);
        printf("Connection established!");

    }else{
        printf("Connection failed\n");
    }
    close(sockfd);
    return 0;
    #else
    n = recv(sockfd, recvline, sizeof(recvline)-1,0);
    //printf("%s",recvline);
    if (n<0){
        printf("read error");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    if(n>0){
        //send an ack packet to complete the connection
        strcpy(sendline,"ACK");
        send(sockfd,sendline,strlen(sendline)+1,0);
        //printf("%s\n",sendline);
        printf("Connection established!\n");

    }else if(n==0){
        printf("connect closed\n");
    }else{
        printf("last Connection failed\n");
    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
    #endif
}

int main(){

    int port_ex =0; // 1 : 1~1024 , 2 : 1 ~ 65535
    char ip_set[16];

    FILE *fp;
    fp = fopen("config.conf","r");

    if (fp==NULL){
        printf("failed!\n");
    }

    fscanf(fp,"%s %d",ip_set,&port_ex);
    printf("%s %d",ip_set,port_ex);
    
    if (port_ex==1){
        for(int i=1; i<=1024; i++){
            socket_(ip_set,i);
        }
    }else if (port_ex==2){
        for(int i=1; i<=65535; i++){
            socket_(ip_set,i);
        }
    }else{
        printf("no port num \n");
        return 0;
    }
    

    return 0;

}