#include <stdio.h>
#include <arpa/inet.h>

#define PORT 6000

int main(){

    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t len;

    sock = socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock,(struct sockaddr*)&server_addr,sizeof(server_addr));

    printf("Subscriber UDP esperando...\n");

    while(1){

        int n = recvfrom(sock,buffer,1024,0,
                        (struct sockaddr*)&server_addr,&len);

        buffer[n]='\0';

        printf("Update: %s\n",buffer);
    }
}