#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <windef.h>







typedef struct{
    int server_num;
    char *ip_;
}ARG;



void Error(const char *msg){
    printf("%s",msg);
}


int PORT_=64;

int non_blocking(int port,char *str_ip){
    
    char *str_ip1;
    str_ip1=str_ip;
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
        server.sin_addr.s_addr =inet_addr(str_ip);
        server.sin_family=AF_INET;
        server.sin_port=htons(port);

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

int run(ARG arg){

    ARG arg1;
    arg1 =arg;
//************************************[socket create]*********************************************************
    int sock_array[FD_SETSIZE]={0}; //포트 검열
    char buf[1024];
    int h=64;

    fd_set read_fds;
    fd_set write_fds;
    fd_set error_fds;

    char *ip_1;
    ip_1=arg1.ip_;
    

    
    printf("%s \n",ip_1);
    memset(sock_array,0,sizeof(sock_array));
    
    
    //64*16 = 1024
    for(int m=0;m<16; m++){
        //socket 배열 순번 set
        int t=0;
        // 64개 먼저 돌아감
        for(int q=PORT_*m; q<PORT_+(m+1); q++){
            sock_array[t]=non_blocking(q,ip_1);
            t=t+1;
            }       
    
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


    //timeout config
    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=50000;

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
            printf("server num : %d ,sock_handle: %d , port : %d\n",arg1.server_num,sock_array[e],e*(m+1));
            if(n==SOCKET_ERROR){
                printf("server num : %d ,recv error\n",arg1.server_num);
                continue;
            }else if(n==0){
                printf("timeout! \n");
                continue;
            }else{
                printf("recv %d bytes!\n");
                printf("server num: %d ,recv string %s \n",arg1.server_num,buf);
            }
        }
        }
    for (int i=0; i<PORT_; i++){
        closesocket(sock_array[i]);
    }
    sleep(1);
    }
    WSACleanup();
    return 0;
}

int server_num(){
    int server_num;

    printf("server num : ");
    scnaf("%d",server_num);

    return server_num;
}

char *server_ip(int server_num){
    int count=server_num;
    char *ip_1;
    printf("ip insert server %d :",count);
    scanf("%s",ip_1);

    return ip_1;

}



int main(void){
    HANDLE hthread1,hthread2;
    DWORD dwThreadId =1;
    ARG arg1;
    ARG arg2;

    char *ip_1 = "192.168.168.128";
    char *ip_2 = "172.23.87.13";
    
    arg1.ip_=ip_1;
    arg2.ip_=ip_2;

    arg1.server_num=1;
    arg2.server_num=2;



    hthread1 = (HANDLE)_beginthreadex(NULL,0,(void*)run,&arg1,0,(unsigned*)&dwThreadId);
    hthread2 = (HANDLE)_beginthreadex(NULL,0,(void*)run,&arg2,0,(unsigned*)&dwThreadId);
    if(hthread1==0 || hthread2 ==0){
        Error("_beginthreadex Error \n");
    }
    
    printf("create thread : %d\n",hthread1);
    printf("create thread : %d\n",hthread2);    
    Sleep(5000);
    printf("The end!\n");
    return 0;

}
// 1. for문으로 쓰레드 돌리기 (3개까지 받기) , 2. port 번호 1024번까지 하기
// 2. for문으로 변수를 생성하는건 불가능 -> 배열로 ip값 받고 그대로 넘김 (ip가 식별자) //포트번호는 일단 고정 (-> sol 한번 찾아봐야함 , 동적 구조체 ? 가능?)
// 동적 구조체 가능 -> 동적 구조체 배열 *malloc