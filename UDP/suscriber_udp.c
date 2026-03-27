#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BROKER_PORT 6000
#define MY_PORT 6001
#define BUFFER 1024

int main() {
    int sock;
    struct sockaddr_in my_addr, broker_addr;
    char buffer[BUFFER];
    socklen_t len = sizeof(broker_addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    memset(&broker_addr, 0, sizeof(broker_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        close(sock);
        exit(1);
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &broker_addr.sin_addr);

    sendto(sock, "SUBSCRIBE", 9, 0,
           (struct sockaddr*)&broker_addr, sizeof(broker_addr));

    printf("Subscriber UDP registrado y esperando mensajes...\n");

    while (1) {
        int n = recvfrom(sock, buffer, BUFFER - 1, 0,
                         NULL, NULL);

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[n] = '\0';
        printf("Update: %s\n", buffer);
    }

    close(sock);
    return 0;
}