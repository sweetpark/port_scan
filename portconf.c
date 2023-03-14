#include <stdio.h>
#include <string.h>



int main(int argc,char **argv){

    //ip config
    char ip_set[20];
    printf("target IP : ");
    scanf("%s",ip_set);

    //port config (1. 1 ~ 1024 2. 1 ~ 65535)
    int num;
    printf("port range ? \n 1. 1 ~ 1024 \n 2. 1 ~ 65535\n 3. quit\n");
    scanf("%d",&num);

    FILE *fp;
    fp = fopen("config.conf","w");

    if(fp==NULL){
        printf("file failed!\n");
        return 0;
    }

    //printf("%s %d\n",ip_set,num);

    if (num==1){
        fprintf(fp, "%s %d",ip_set,num);
        printf("success\n");
    }else if(num==2){
        fprintf(fp, "%s %d",ip_set,num);
        printf("success\n");
    }else{
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 0;
}