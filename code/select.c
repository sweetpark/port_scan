#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>

int PORT_=50;

int non_blocking(int port){
    
    
    u_long n=1; //non blocking mode

    WSADATA wsa;
    if (WSAStartup((2,2),&wsa) !=0){
        printf("WSAStartup failed\n");
        return 1;
    }
    SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock == INVALID_SOCKET){
            printf("socket creation failed\n");
            return 1;
    }

        struct sockaddr_in server;
        server.sin_addr.s_addr =inet_addr("192.168.168.128");
        server.sin_family=AF_INET;
        server.sin_port=htons(port);

        ioctlsocket(sock,FIONBIO, &n); //non blocking -> no wait connect()
        if (connect(sock, (struct sockaddr*)&server,sizeof(server))==SOCKET_ERROR){
                //printf("connect fail\n");
                // closesocket(sock);
                // WSACleanup();
                // return 1;        
        }
    
    return sock;


    //***********************************[socket create]**********************************************


}


int main(void){



//************************************[socket create]*********************************************************
    int sock_array[FD_SETSIZE]={0}; //포트 검열
    char buf[1024];
    fd_set read_fds;
    fd_set write_fds;
    fd_set error_fds;



    //for (int q=0; q<PORT_+1; q++){
        memset(sock_array,0,sizeof(sock_array));
    //non_blocking send    
        //for(int w=0; w<FD_SETSIZE+1; w++){
        for(int q=0; q<PORT_+1; q++){
        sock_array[q]=non_blocking(q);
        //printf("sock num : %d",sock_array[q]);
        }        //printf("sock : sock_array %d",sock_array[w]);
        //}

        //select 초기화
        for(int j=0; j<1; j++){
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            FD_ZERO(&error_fds);
        }

        //select insert
        for(int k=0;k<FD_SETSIZE+1; k++){
            FD_SET((u_int)sock_array[k] ,&read_fds);
            FD_SET((u_int)sock_array[k] ,&write_fds);
            FD_SET((u_int)sock_array[k] ,&error_fds);
        }

        //while(1){
            //timeout config
            struct timeval timeout;
            timeout.tv_sec=0;
            timeout.tv_usec=20000;

            //select value => wset 예외set 추가
            int ready_fds=select(FD_SETSIZE, &read_fds,&write_fds,&error_fds,&timeout);

            if(ready_fds == SOCKET_ERROR){
                printf("select error\n");
                //break;
            }

            if(ready_fds ==0){
                printf("timeout occured\n");
               // continue;
            }
            //select value 있다면,
            for(int e=0; e<FD_SETSIZE+1; e++){
                if(FD_ISSET(sock_array[e], &read_fds) ||FD_ISSET(sock_array[e], &write_fds ) ||FD_ISSET(sock_array[e], &error_fds)){
                    
                    int n = recv(sock_array[e],buf,sizeof(buf),0);
                    printf("%d , port : %d\n",sock_array[e],e);
                    if(n==SOCKET_ERROR){
                        printf("recv error\n");
                        continue;
                    }else if(n==0){
                        printf("timeout! \n");
                        continue;
                    }else{
                        printf("recv %d bytes!\n");
                        printf("recv string %s \n",buf);
                    }
                }
            }
        //}
    //}
    for (int i=0; i<PORT_+1; i++){
        closesocket(sock_array[i]);
    }
    WSACleanup();
    return 0;

}
