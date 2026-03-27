#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 6000

int main(){

    int sock;
    struct sockaddr_in server_addr;
    char message[1024];

    sock = socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);

    while(1){

        printf("Evento: ");
        fgets(message,1024,stdin);

        sendto(sock,message,strlen(message),0,
              (struct sockaddr*)&server_addr,sizeof(server_addr));
    }
}