#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BROKER_PORT 6000
#define BUFFER 1024

int main() {
    int sock;
    struct sockaddr_in broker_addr;
    char message[BUFFER];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &broker_addr.sin_addr);

    while (1) {
        printf("Evento: ");
        fgets(message, BUFFER, stdin);

        sendto(sock, message, strlen(message), 0,
               (struct sockaddr*)&broker_addr, sizeof(broker_addr));
    }

    close(sock);
    return 0;
}