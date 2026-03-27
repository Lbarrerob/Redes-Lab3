#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6000
#define BUFFER 1024
#define MAX_CLIENTS 20

struct sockaddr_in subscribers[MAX_CLIENTS];
int subscriber_count = 0;

int subscriber_exists(struct sockaddr_in *client) {
    for (int i = 0; i < subscriber_count; i++) {
        if (subscribers[i].sin_addr.s_addr == client->sin_addr.s_addr &&
            subscribers[i].sin_port == client->sin_port) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER];
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("Broker UDP escuchando en puerto %d...\n", PORT);

    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER - 1, 0,
                         (struct sockaddr*)&client_addr, &len);

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[n] = '\0';

        printf("Recibido de %s:%d -> %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        if (strncmp(buffer, "SUBSCRIBE", 9) == 0) {
            if (!subscriber_exists(&client_addr) && subscriber_count < MAX_CLIENTS) {
                subscribers[subscriber_count++] = client_addr;
                printf("Nuevo subscriber registrado: %s:%d\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port));
            }
        } else {
            for (int i = 0; i < subscriber_count; i++) {
                sendto(sockfd, buffer, strlen(buffer), 0,
                       (struct sockaddr*)&subscribers[i],
                       sizeof(subscribers[i]));
            }
        }
    }

    close(sockfd);
    return 0;
}