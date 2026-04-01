#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/*
 * publisher_tcp.c
 *
 * Cliente publisher TCP.
 * - Se conecta al broker en 127.0.0.1:5000.
 * - Lee eventos desde stdin.
 * - Envia cada evento al broker.
 */

#define PORT 5000

int main() {

    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    /* Captura y publica eventos en un ciclo infinito. */
    while(1) {
        printf("Evento: ");
        fgets(message,1024,stdin);

        send(sock,message,strlen(message),0);
    }
}