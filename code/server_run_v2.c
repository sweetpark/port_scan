#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <windef.h>
#include <string.h>


void Error(const char *msg){
    printf("%s",msg);
}


int PORT_=64;

int non_blocking(int port,char *str_ip){
    
    char *str_ip1; //non_blocking ip
    str_ip1=str_ip;
    u_long n=1; //non blocking mode
    WSADATA wsa;


    if (WSAStartup((2,2),&wsa) !=0){
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET,SOCK_STREAM,0); //socket

    if (sock == INVALID_SOCKET){
            printf("socket creation failed\n");
            return 1;
    }

        struct sockaddr_in server;
        server.sin_addr.s_addr =inet_addr(str_ip1);
        server.sin_family=AF_INET;
        server.sin_port=htons(port);
        //printf("non blocking port : %d\n",port);
        ioctlsocket(sock,FIONBIO, &n); //non blocking -> no wait connect()
        if (connect(sock, (struct sockaddr*)&server,sizeof(server))==SOCKET_ERROR){
                // printf("connect fail\n");
                // closesocket(sock);
                // WSACleanup();
                // return 1;        
        }
    
    return sock;


    //***********************************[socket create]**********************************************


}

int run(char *ip){


//************************************[socket create]*********************************************************
    int sock_array[FD_SETSIZE]={0}; //포트 검열
    char buf[1024];
    int z=0; //FD_ISSET check num
    int port_num=0; // port_number
    int count=0; // port_num_range check


    fd_set read_fds;
    fd_set write_fds;
    fd_set error_fds;

    char *ip_1=ip; //ip
    // DWORD err;
    // err=GetLastError();
    

    
    //printf("%s \n",ip_1);
    //memset(sock_array,0,sizeof(sock_array));

    //64*16 = 1024
    for(int m=0;m<16; m++){
        //socket 배열 순번 set
        int t=0; // socket 배열값 0~64 반복
        //timeout config
        struct timeval timeout;
        timeout.tv_sec=0;
        timeout.tv_usec=50000;

        // 64개 먼저 돌아감
        for(int q=64*m; q<64*(m+1); q++){
          // printf("port num : %d\n",q);
            Sleep(200);
            sock_array[t]=non_blocking(q,ip_1);
            t=t+1; //(0~63)
            }       


        //select value => wset 예외set 추가
       for(int j=0; j<3; j++){
                FD_ZERO(&read_fds);
                FD_ZERO(&write_fds);
                FD_ZERO(&error_fds);
                }

            //select insert
        for(int k=0;k<FD_SETSIZE; k++){
                FD_SET((u_int)sock_array[k] ,&read_fds);
                FD_SET((u_int)sock_array[k] ,&write_fds);
                FD_SET((u_int)sock_array[k] ,&error_fds);
                }
        
       int ready_fds=select(0, &read_fds,&write_fds,&error_fds,&timeout);
       //select 초기화

        if(ready_fds == SOCKET_ERROR){
            printf("select error\n");
            //break;
            }

        // if(ready_fds ==0){
        //     printf("timeout occured!! ip: %s ,port_num : %d\n",ip_1,port_num);
        //     //continue;
        //     }
    
        //select value 있다면,
        for(int e=0; e<FD_SETSIZE; e++){
            z=0;
            //printf("%d port num\n",port_num);
            while(z<3){
                if(FD_ISSET(sock_array[e], &read_fds) ||FD_ISSET(sock_array[e], &write_fds ) ||FD_ISSET(sock_array[e], &error_fds)){
                    if (FD_ISSET(sock_array[e], &read_fds)) {printf("ip: %s , R_port : %d\n",ip_1,port_num); break;} //recv 받을시 사용
                    if (FD_ISSET(sock_array[e], &write_fds)) {printf("ip: %s , w_port : %d\n",ip_1,port_num); break;} //send 보낼시
                    
                    /*recv 배너값 받기 (send data의 경우 recv 없음)   ex_ http     */
                    //if (FD_ISSET(sock_array[e], &error_fds)) {continue;}
                    //int n = recv(sock_array[e],buf,sizeof(buf),0);
                    //printf("sock_handle: %d\n",sock_array[e]);
                    // if(n<0/*SOCKET_ERROR*/){
                    //     printf("recv error\n");
                    //     printf("%ld\n",GetLastError());
                    //     continue;}
                    // }else if(n==0){
                    //     printf("timeout! \n");
                    //     continue;
                    // }else{
                    //     Sleep(200);
                    //     printf("recv %d bytes!\n",n);
                    //     printf("recv string %s \n",buf);
                    // }
        
                    }
                z++;
                }
                port_num++;
                count++;
            }
    
    printf("\n[ip: %s, (%d ~ %d finish port scan)]\n\n",ip_1,count-64,count); // port range print
    for (int i=0; i<PORT_; i++){
        closesocket(sock_array[i]);
        }
    //sleep(1);
        WSACleanup();
    }
    //다음 port 번호로 
    return 0;
}

int server_count(){
    int server_num;

    printf("server num : ");
    scanf(" %d",&server_num);

    return server_num;
}

    // test ip
    // "192.168.168.128";
    // "172.23.87.13";


int main(void){
    HANDLE hthread,hthread2;
    DWORD dwThreadId =1;
    /*서버 개수 등록 */
    int server_num=0;
    server_num=server_count();
    //printf("%d\n",server_num);

    /*서버 ip 등록*/
    char tmp[100];
    char *ip[server_num];
    
    for(int i=0; i<server_num; i++){
        printf("\n##############################################################\n");
        printf("ip insert server %d :",i+1);
        scanf(" %s",tmp);
        printf("##############################################################\n");

        ip[i]=(char*)malloc(strlen(tmp)+1);

        strcpy(ip[i],tmp);
        //printf(" %s",ip[i]);
    }


    //thread.exe
    for(int k=0; k<server_num; k++){
        hthread = (HANDLE)_beginthreadex(NULL,0,(void*)run,ip[k],0,(unsigned*)&dwThreadId);
    }

    if(hthread==0){
        Error("_beginthreadex Error \n");
    }
    printf("\ncreate thread\n");
    
    
    printf("\n!main end!\n!!!!!!!!!!!!do you want end -> please insert \"quit\"!!!!!!!!!!!!!!!\n[playing port scan...] \n\n");
    while(TRUE){
        scanf(" %s",tmp);
        if(strcpy(tmp,"quit")){
            break;
        }
    }
    printf("The end!\n");
    
    //동적할당 해제
    for (int i=0; i<server_num; i++){
        free(ip[i]);
    }
    return 0;

}
// 1. for문으로 쓰레드 돌리기 (3개까지 받기) , 2. port 번호 1024번까지 하기
// 2. for문으로 변수를 생성하는건 불가능 -> 배열로 ip값 받고 그대로 넘김 (ip가 식별자) //포트번호는 일단 고정 (-> sol 한번 찾아봐야함 , 동적 구조체 ? 가능?)
// 동적 구조체 가능 -> 동적 구조체 배열 *malloc
// while=True