#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 6000
#define BUFFER 1024
#define MAX_CLIENTS 10

struct sockaddr_in clients[MAX_CLIENTS];
int client_count = 0;

int main(){

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER];
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

    printf("Broker UDP listening...\n");

    while(1){

        int n = recvfrom(sockfd,buffer,BUFFER,0,
                        (struct sockaddr*)&client_addr,&len);

        buffer[n]='\0';

        printf("Message: %s\n",buffer);

        clients[client_count++] = client_addr;

        for(int i=0;i<client_count;i++){

            sendto(sockfd,buffer,strlen(buffer),0,
                   (struct sockaddr*)&clients[i],sizeof(clients[i]));
        }
    }
}